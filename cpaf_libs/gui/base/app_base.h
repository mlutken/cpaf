#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <fmt/format.h>
#include <cpaf_libs/gui/gui_types.h>
#include <cpaf_libs/gui/system_window.h>


namespace cpaf::gui {



class app_base {
public:

    app_base() = default;
    app_base(const app_base&) = delete;
    app_base(app_base&&) = delete;
    app_base& operator=(const app_base& other) = delete;
    app_base& operator=(app_base&& other) = delete;

    std::string                     dbg_characteristics         () const;
    exit_status_t                   run                         ();

    std::unique_ptr<system_window_base>  create_system_window   (size_2d size, std::string_view title) const {
        fmt::println("FIXMENM create_system_window () ");
        return do_create_system_window(size, title);
    };

    void                    app_name_set        (const std::string_view name)      { app_name_    = name;     }
    void                    company_name_set    (const std::string_view name)      { company_name_= name;     }
    void                    window_title_set    (const std::string_view title)     { window_title_= title;    }

    const std::string_view  app_name            () const     { return app_name_;        }
    const std::string_view  company_name        () const     { return company_name_;    }
    const std::string_view  window_title        () const     { return window_title_;    }

protected:
    exit_status_t                   exit_status_                    = exit_status_t::success;
    std::unique_ptr<system_window_base>  main_window_                    = nullptr;
    // --- PROTECTED: Helper functions for derived classes ---

private:

    virtual void                    do_run						() = 0;
    virtual void                    do_pre_frame_update         () = 0;
    virtual void                    do_frame_update             () = 0;
    virtual void                    do_post_frame_update		() = 0;
    virtual std::unique_ptr<system_window_base>  do_create_system_window        (size_2d size, std::string_view title) const = 0;

    std::string               app_name_         {"MyApp"};
    std::string               company_name_     {""};
    std::string               window_title_     {"Hello CPAF GUI"};

};

} //END namespace cpaf::gui

