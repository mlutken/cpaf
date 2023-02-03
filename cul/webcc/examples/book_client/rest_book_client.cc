#include <iostream>
#include <list>

#include "boost/algorithm/string/predicate.hpp"
#include "boost/filesystem/operations.hpp"

#include "json/json.h"

#include "webcc/client_session.h"
#include "webcc/logger.h"

#include "examples/common/book.h"
#include "examples/common/book_json.h"

#if (defined(_WIN32) || defined(_WIN64))
#if defined(_DEBUG) && defined(WEBCC_ENABLE_VLD)
#pragma message ("< include vld.h >")
#include "vld/vld.h"
#pragma comment(lib, "vld")
#endif
#endif

namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------

class BookClient {
public:
  explicit BookClient(const std::string& url, int timeout = 0);

  ~BookClient() = default;

  bool ListBooks(std::list<Book>* books);

  bool CreateBook(const std::string& title, double price,
                  const bfs::path& photo, std::string* id);

  bool GetBook(const std::string& id, Book* book);

  bool UpdateBook(const std::string& id, const std::string& title,
                  double price);

  bool DeleteBook(const std::string& id);

private:
  bool CheckPhoto(const bfs::path& photo);

  // Check HTTP response status.
  bool CheckStatus(webcc::ResponsePtr response, int expected_status);

private:
  std::string url_;
  webcc::ClientSession session_;
};

// -----------------------------------------------------------------------------

BookClient::BookClient(const std::string& url, int timeout)
    : url_(url), session_(timeout) {
  // If the request has body, default to this content type.
  // Optional.
  session_.set_media_type("application/json");
  session_.set_charset("utf-8");
}

bool BookClient::ListBooks(std::list<Book>* books) {
  try {
    auto r = session_.Send(WEBCC_GET(url_).Path("books")());

    if (!CheckStatus(r, webcc::Status::kOK)) {
      // Response HTTP status error.
      return false;
    }

    Json::Value rsp_json = StringToJson(r->data());

    if (!rsp_json.isArray()) {
      return false;  // Should be a JSON array of books.
    }

    for (Json::ArrayIndex i = 0; i < rsp_json.size(); ++i) {
      books->push_back(JsonToBook(rsp_json[i]));
    }

    return true;

  } catch (const webcc::Error& error) {
    std::cerr << error << std::endl;
    return false;
  }
}

bool BookClient::CreateBook(const std::string& title, double price,
                            const bfs::path& photo, std::string* id) {
  Json::Value req_json;
  req_json["title"] = title;
  req_json["price"] = price;

  try {
    auto r = session_.Send(WEBCC_POST(url_).Path("books").
                           Body(JsonToString(req_json))());

    if (!CheckStatus(r, webcc::Status::kCreated)) {
      return false;
    }

    Json::Value rsp_json = StringToJson(r->data());
    *id = rsp_json["id"].asString();

    if (id->empty()) {
      return false;
    }

    if (CheckPhoto(photo)) {
      r = session_.Send(WEBCC_PUT(url_).Path("books").Path(*id).Path("photo").
                        File(photo)());

      if (!CheckStatus(r, webcc::Status::kOK)) {
        return false;
      }
    }

    return true;

  } catch (const webcc::Error& error) {
    std::cerr << error << std::endl;
    return false;
  }
}

bool BookClient::GetBook(const std::string& id, Book* book) {
  try {
    auto r = session_.Send(WEBCC_GET(url_).Path("books").Path(id)());

    if (!CheckStatus(r, webcc::Status::kOK)) {
      return false;
    }

    return JsonStringToBook(r->data(), book);

  } catch (const webcc::Error& error) {
    std::cerr << error << std::endl;
    return false;
  }
}

bool BookClient::UpdateBook(const std::string& id, const std::string& title,
                            double price) {
  Json::Value json;
  json["title"] = title;
  json["price"] = price;

  try {
    auto r = session_.Send(WEBCC_PUT(url_).Path("books").Path(id).
                           Body(JsonToString(json))());

    if (!CheckStatus(r, webcc::Status::kOK)) {
      return false;
    }

    return true;

  } catch (const webcc::Error& error) {
    std::cerr << error << std::endl;
    return false;
  }
}

bool BookClient::DeleteBook(const std::string& id) {
  try {
    auto r = session_.Send(WEBCC_DELETE(url_).Path("books").Path(id)());

    if (!CheckStatus(r, webcc::Status::kOK)) {
      return false;
    }

    return true;

  } catch (const webcc::Error& error) {
    std::cerr << error << std::endl;
    return false;
  }
}

bool BookClient::CheckPhoto(const bfs::path& photo) {
  if (photo.empty()) {
    return false;
  }

  if (!bfs::is_regular_file(photo) || !bfs::exists(photo)) {
    return false;
  }

  auto ext = photo.extension().string();
  if (!boost::iequals(ext, ".jpg") && !boost::iequals(ext, ".jpeg")) {
    return false;
  }

  return true;
}

bool BookClient::CheckStatus(webcc::ResponsePtr response, int expected_status) {
  if (response->status() != expected_status) {
    LOG_ERRO("HTTP status error (actual: %d, expected: %d).",
             response->status(), expected_status);
    return false;
  }
  return true;
}

// -----------------------------------------------------------------------------

void PrintSeparator() {
  static const std::string s_line(80, '-');
  std::cout << s_line << std::endl;
}

void PrintBook(const Book& book) {
  std::cout << "Book: " << book << std::endl;
}

void PrintBookList(const std::list<Book>& books) {
  std::cout << "Book list: " << books.size() << std::endl;
  for (const Book& book : books) {
    std::cout << "  Book: " << book << std::endl;
  }
}

// -----------------------------------------------------------------------------

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cout << "usage: rest_book_client <url>" << std::endl;
    std::cout << "examples:" << std::endl;
    std::cout << "  $ rest_book_client http://localhost:8080" << std::endl;
    return 1;
  }

  std::string url = argv[1];

  WEBCC_LOG_INIT("", webcc::LOG_CONSOLE_FILE_OVERWRITE);

  BookClient client(url);

  PrintSeparator();

  // List all books.

  std::list<Book> books;
  if (client.ListBooks(&books)) {
    PrintBookList(books);
  } else {
    return 1;
  }

  PrintSeparator();

  // Create a new book.

  std::string id;
  if (client.CreateBook("1984", 12.3, "", &id)) {
    std::cout << "Book ID: " << id << std::endl;
  } else {
    return 1;
  }

  PrintSeparator();

  books.clear();
  if (client.ListBooks(&books)) {
    PrintBookList(books);
  } else {
    return 1;
  }

  PrintSeparator();

  Book book;
  if (client.GetBook(id, &book)) {
    PrintBook(book);
  } else {
    return 1;
  }

  PrintSeparator();

  if (!client.UpdateBook(id, "1Q84", 32.1)) {
    return 1;
  }

  PrintSeparator();

  if (client.GetBook(id, &book)) {
    PrintBook(book);
  } else {
    return 1;
  }

  PrintSeparator();

  if (!client.DeleteBook(id)) {
    return 1;
  }

  PrintSeparator();

  books.clear();
  if (client.ListBooks(&books)) {
    PrintBookList(books);
  }

  std::cout << "Press any key to exit: ";
  std::getchar();

  return 0;
}
