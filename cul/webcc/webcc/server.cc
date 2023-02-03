#include "webcc/server.h"

#include <csignal>
#include <fstream>
#include <utility>

#include "boost/filesystem/operations.hpp"

#include "webcc/body.h"
#include "webcc/logger.h"
#include "webcc/request.h"
#include "webcc/response.h"
#include "webcc/utility.h"

namespace bfs = boost::filesystem;

using tcp = boost::asio::ip::tcp;

namespace webcc {

Server::Server(boost::asio::ip::tcp protocol, std::uint16_t port,
               const bfs::path& doc_root)
    : protocol_(protocol),
      port_(port),
      doc_root_(doc_root),
      file_chunk_size_(1024),
      running_(false),
      acceptor_(io_context_),
      signals_(io_context_) {
  AddSignals();
}

void Server::Run(std::size_t workers, std::size_t loops) {
  assert(workers > 0);

  {
    std::lock_guard<std::mutex> lock(state_mutex_);

    assert(worker_threads_.empty());

    if (IsRunning()) {
      LOG_WARN("Server is already running.");
      return;
    }

    running_ = true;
    io_context_.restart();

    if (!Listen(port_)) {
      LOG_ERRO("Server is NOT going to run.");
      return;
    }

    LOG_INFO("Server is going to run...");

    AsyncWaitSignals();

    AsyncAccept();

    // Create worker threads.
    for (std::size_t i = 0; i < workers; ++i) {
      worker_threads_.emplace_back(&Server::WorkerRoutine, this);
    }
  }

  // Start the event loop.
  // The io_context::run() call will block until all asynchronous operations
  // have finished. While the server is running, there is always at least one
  // asynchronous operation outstanding: the asynchronous accept call waiting
  // for new incoming connections.

  LOG_INFO("Loop is running in %u thread(s).", loops);

  if (loops == 1) {
    // Run the loop in current thread.
    io_context_.run();
  } else {
    std::vector<std::thread> loop_threads;
    for (std::size_t i = 0; i < loops; ++i) {
      loop_threads.emplace_back(&boost::asio::io_context::run, &io_context_);
    }
    // Join the threads for blocking.
    for (std::size_t i = 0; i < loops; ++i) {
      loop_threads[i].join();
    }
  }
}

void Server::Stop() {
  std::lock_guard<std::mutex> lock(state_mutex_);

  DoStop();
}

bool Server::IsRunning() const {
  return running_ && !io_context_.stopped();
}

void Server::AddSignals() {
  signals_.add(SIGINT);  // Ctrl+C
  signals_.add(SIGTERM);

#if defined(SIGQUIT)
  signals_.add(SIGQUIT);
#endif
}

void Server::AsyncWaitSignals() {
  signals_.async_wait(
      [this](boost::system::error_code, int signo) {
        // The server is stopped by canceling all outstanding asynchronous
        // operations. Once all operations have finished the io_context::run()
        // call will exit.
        LOG_INFO("On signal %d, stopping the server...", signo);

        DoStop();
      });
}

bool Server::Listen(std::uint16_t port) {
  boost::system::error_code ec;

  tcp::endpoint endpoint(protocol_, port);

  // Open the acceptor.
  acceptor_.open(endpoint.protocol(), ec);
  if (ec) {
    LOG_ERRO("Acceptor open error (%s).", ec.message().c_str());
    return false;
  }

  // Set option SO_REUSEADDR on.
  // When SO_REUSEADDR is set, multiple servers can listen on the same port.
  // This is necessary for restarting the server on the same port.
  // More details:
  // - https://stackoverflow.com/a/3233022
  // - http://www.andy-pearce.com/blog/posts/2013/Feb/so_reuseaddr-on-windows/
  acceptor_.set_option(tcp::acceptor::reuse_address(true));

  // Bind to the server address.
  acceptor_.bind(endpoint, ec);
  if (ec) {
    LOG_ERRO("Acceptor bind error (%s).", ec.message().c_str());
    return false;
  }

  // Start listening for connections.
  // After listen, the client is able to connect to the server even the server
  // has not started to accept the connection yet.
  acceptor_.listen(boost::asio::socket_base::max_listen_connections, ec);
  if (ec) {
    LOG_ERRO("Acceptor listen error (%s).", ec.message().c_str());
    return false;
  }

  return true;
}

void Server::AsyncAccept() {
  acceptor_.async_accept(
      [this](boost::system::error_code ec, tcp::socket socket) {
        // Check whether the server was stopped by a signal before this
        // completion handler had a chance to run.
        if (!acceptor_.is_open()) {
          return;
        }

        if (!ec) {
          LOG_INFO("Accepted a connection.");

          using namespace std::placeholders;
          auto view_matcher = std::bind(&Server::MatchViewOrStatic, this, _1,
                                        _2, _3);

          auto connection = std::make_shared<Connection>(
              std::move(socket), &pool_, &queue_, std::move(view_matcher));

          pool_.Start(connection);
        }

        AsyncAccept();
      });
}

void Server::DoStop() {
  // Stop accepting new connections.
  acceptor_.close();

  // Stop worker threads.
  // This might take some time if the threads are still processing.
  StopWorkers();

  // Close all pending connections.
  pool_.Clear();

  // Finally, stop the event processing loop.
  // This function does not block, but instead simply signals the io_context to
  // stop. All invocations of its run() or run_one() member functions should
  // return as soon as possible.
  io_context_.stop();

  running_ = false;
}

void Server::WorkerRoutine() {
  LOG_INFO("Worker is running.");

  for (;;) {
    auto connection = queue_.PopOrWait();

    if (!connection) {
      LOG_INFO("Worker is going to stop.");

      // For stopping next worker.
      queue_.Push(ConnectionPtr());

      // Stop this worker.
      break;
    }

    Handle(connection);
  }
}

void Server::StopWorkers() {
  LOG_INFO("Stopping workers...");

  // Clear/drop pending connections.
  // The connections will be closed later (see DoStop).
  // Alternatively, we can wait for the pending connections to be handled.
  if (queue_.Size() != 0) {
    LOG_INFO("Clear pending connections...");
    queue_.Clear();
  }

  // Enqueue a null connection to trigger the first worker to stop.
  queue_.Push(ConnectionPtr());

  // Wait for worker threads to finish.
  for (auto& t : worker_threads_) {
    if (t.joinable()) {
      t.join();
    }
  }

  // Cleanup worker threads.
  worker_threads_.clear();

  // Clear the queue because it has a remaining null connection pushed by the
  // last worker thread.
  queue_.Clear();

  LOG_INFO("All workers have been stopped.");
}

void Server::Handle(ConnectionPtr connection) {
  auto request = connection->request();

  const Url& url = request->url();
  LOG_INFO("Request URL path: %s", url.path().c_str());

  UrlArgs args;
  auto view = FindView(request->method(), url.path(), &args);

  if (!view) {
    LOG_WARN("No view matches the request: %s %s", request->method().c_str(),
             url.path().c_str());

    if (request->method() == methods::kGet) {
      // Try to serve static files for GET request.
      auto response = ServeStatic(request);
      if (!response) {
        // Static file not found.
        connection->SendResponse(Status::kNotFound);
      } else {
        connection->SendResponse(response);
      }
    } else {
      connection->SendResponse(Status::kBadRequest);
    }

    return;
  }

  // Save the (regex matched) URL args to request object.
  request->set_args(args);

  // Ask the matched view to process the request.
  ResponsePtr response = view->Handle(request);

  // Send the response back.
  if (response) {
    connection->SendResponse(response);
  } else {
    connection->SendResponse(Status::kBadRequest);
  }
}

bool Server::MatchViewOrStatic(const std::string& method,
                               const std::string& url, bool* stream) {
  if (Router::MatchView(method, url, stream)) {
    return true;
  }

  // Try to match a static file.
  if (method == methods::kGet && !doc_root_.empty()) {
    bfs::path path = doc_root_ / url;
    if (!bfs::is_directory(path) && bfs::exists(path)) {
      return true;
    }
  }

  return false;
}

ResponsePtr Server::ServeStatic(RequestPtr request) {
  assert(request->method() == methods::kGet);

  if (doc_root_.empty()) {
    LOG_INFO("The doc root was not specified.");
    return {};
  }

  bfs::path path = doc_root_ / request->url().path();

  try {
    // NOTE: FileBody might throw Error::kFileError.
    auto body = std::make_shared<FileBody>(path, file_chunk_size_);

    auto response = std::make_shared<Response>(Status::kOK);

    std::string extension = path.extension().string();
    response->SetContentType(media_types::FromExtension(extension), "");

    // NOTE: Gzip compression is not supported.
    response->SetBody(body, true);

    return response;

  } catch (const Error& error) {
    LOG_ERRO("File error: %s.", error.message().c_str());
    return {};
  }
}

}  // namespace webcc
