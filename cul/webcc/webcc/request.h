#ifndef WEBCC_REQUEST_H_
#define WEBCC_REQUEST_H_

#include <memory>
#include <string>
#include <vector>

#include "webcc/message.h"
#include "webcc/url.h"

namespace webcc {

class Request : public Message {
public:
  Request() = default;

  explicit Request(const std::string& method) : method_(method) {
  }

  ~Request() override = default;

  const std::string& method() const {
    return method_;
  }

  void set_method(const std::string& method) {
    method_ = method;
  }

  const Url& url() const {
    return url_;
  }

  void set_url(Url&& url) {
    url_ = std::move(url);
  }

  const std::string& host() const {
    return url_.host();
  }

  const std::string& port() const {
    return url_.port();
  }

  UrlQuery query() const {
    return UrlQuery(url_.query());
  }

  const UrlArgs& args() const {
    return args_;
  }

  void set_args(const UrlArgs& args) {
    args_ = args;
  }

  const std::string& ip() const {
    return ip_;
  }

  void set_ip(const std::string& ip) {
    ip_ = ip;
  }

  // Check if the body is a multi-part form data.
  bool IsForm() const;

  // Get the form parts from the body.
  // Only applicable to FormBody (i.e., multi-part form data).
  // Otherwise, exception Error(kDataError) will be thrown.
  const std::vector<FormPartPtr>& form_parts() const;

  void Prepare() override;

private:
  std::string method_;

  Url url_;

  // The URL regex matched arguments (usually resource ID's).
  // Used by server only.
  UrlArgs args_;

  // Client IP address.
  std::string ip_;
};

using RequestPtr = std::shared_ptr<Request>;

}  // namespace webcc

#endif  // WEBCC_REQUEST_H_
