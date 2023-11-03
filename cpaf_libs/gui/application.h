#pragma once

#include <SDL2/SDL.h>

#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include <cpaf_libs/gui/window.h>
#include <cpaf_libs/gui/utils.h>
#include <cpaf_libs/gui/gui_types.h>

namespace cpaf::gui {


class application {
public:
    application();
    explicit application(const std::string_view name, const std::string_view window_title);
    ~application();

    application(const application&) = delete;
    application(application&&) = delete;
    application& operator=(application other) = delete;
    application& operator=(application&& other) = delete;

    void                    app_name            (const std::string_view name)      { app_name_    = name;     }
    void                    company_name        (const std::string_view name)      { company_name_= name;     }
    void                    window_title        (const std::string_view title)     { window_title_= title;    }

    const std::string_view  app_name            () const     { return app_name_;        }
    const std::string_view  company_name        () const     { return company_name_;    }
    const std::string_view  window_title        () const     { return window_title_;    }


    exit_status_t run();
    void stop();

//    virtual std::string company_name() const { return "MyCompany"; }

    void on_event(const SDL_WindowEvent& event);
    void on_minimize();
    void on_shown();
    void on_close();

private:
    std::string               app_name_         {"MyApp"};
    std::string               company_name_     {""};
    std::string               window_title_     {"Hello CPAF GUI"};
    exit_status_t             m_exit_status     {exit_status_t::success};
    std::unique_ptr<window>   m_window          {nullptr};

    bool                      m_running         {true};
    bool                      m_minimized       {false};
    bool                      m_show_some_panel {true};
    bool                      m_show_debug_panel{false};
};

} // namespace capf::gui
