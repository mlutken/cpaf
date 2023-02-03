#include "webcc/logger.h"
#include "webcc/response_builder.h"
#include "webcc/server.h"

class HelloView : public webcc::View {
public:
  HelloView(int sleep_seconds) : sleep_seconds_(sleep_seconds) {
  }

  webcc::ResponsePtr Handle(webcc::RequestPtr request) override {
    if (sleep_seconds_ > 0) {
      std::this_thread::sleep_for(std::chrono::seconds(sleep_seconds_));
    }

    if (request->method() == "GET") {
      return webcc::ResponseBuilder{}.OK().Body("Hello, World!")();
    }

    return {};
  }

private:
  int sleep_seconds_;
};

int main(int argc, const char* argv[]) {
  WEBCC_LOG_INIT("", webcc::LOG_CONSOLE);

  int workers = 1;
  int sleep_seconds = 0;

  if (argc > 1) {
    workers = std::stoi(argv[1]);
    if (argc > 2) {
      sleep_seconds = std::stoi(argv[2]);
    }
  }

  LOG_USER("Workers: %d", workers);
  LOG_USER("Sleep seconds: %d", sleep_seconds);

  try {
    webcc::Server server{ boost::asio::ip::tcp::v4(), 8080 };

    auto view = std::make_shared<HelloView>(sleep_seconds);
    server.Route("/", view);
    server.Route("/hello", view);

    server.Run(workers);

  } catch (const std::exception&) {
    return 1;
  }

  return 0;
}
