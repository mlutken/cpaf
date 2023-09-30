#pragma once

#include <SDL2/SDL.h>

#include <memory>
#include <string>
#include <vector>

#include <cpaf_libs/gui/window.h>

namespace cpaf::gui {

enum class ExitStatus : int { SUCCESS = 0, FAILURE = 1 };

class Application {
 public:
  explicit Application(const std::string& title);
  ~Application();

  Application(const Application&) = delete;
  Application(Application&&) = delete;
  Application& operator=(Application other) = delete;
  Application& operator=(Application&& other) = delete;

  ExitStatus run();
  void stop();

  virtual std::string company_name() const { return "MyCompany"; }
  virtual std::string app_name() const { return "MyApp"; }

  void on_event(const SDL_WindowEvent& event);
  void on_minimize();
  void on_shown();
  void on_close();

 private:
  ExitStatus m_exit_status{ExitStatus::SUCCESS};
  std::unique_ptr<window> m_window{nullptr};

  bool m_running{true};
  bool m_minimized{false};
  bool m_show_some_panel{true};
  bool m_show_debug_panel{false};
};

} // namespace capf::gui
