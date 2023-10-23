#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <atomic>
#include <fmt/format.h>
#include <cpaf_libs/gui/gui_types.h>
#include <cpaf_libs/gui/system_window.h>
#include <cpaf_libs/gui/events/event.h>


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
    size_2d                         main_window_size            () const  { return do_main_window_size(); }

    std::unique_ptr<system_window_base>  create_system_window   (size_2d size, std::string_view title) const {
        fmt::println("FIXMENM create_system_window () ");
        return do_create_system_window(size, title);
    };

    void                    stop                ()                                  { is_running_ = false;     }
    void                    app_name_set        (const std::string_view name)       { app_name_    = name;     }
    void                    company_name_set    (const std::string_view name)       { company_name_= name;     }
    void                    window_title_set    (const std::string_view title)      { window_title_= title;    }
    // void                    initial_window_set  (size_2d size, display::window_state wnd_state);

    const std::string_view  app_name            () const     { return app_name_;        }
    const std::string_view  company_name        () const     { return company_name_;    }
    const std::string_view  window_title        () const     { return window_title_;    }

protected:
    std::unique_ptr<system_window_base>     main_window_                    = nullptr;
    size_2d                                 initial_window_size_            = {800, 600};
    std::atomic_bool                        is_running_                     = false;
    exit_status_t                           exit_status_                    = exit_status_t::success;
    // display::window_state                   initial_window_state_           = display::window_state::normal;

    // --- PROTECTED: User virtual functions for derived classes ---
    virtual events::is_handled              event_handler				(const events::event& evt);


    // --- PROTECTED: Helper functions for derived classes ---

private:

    virtual void                    do_start_run				() = 0;
    virtual events::event           do_get_event                () const = 0;
    virtual void                    do_process_events           () = 0;
    virtual void                    do_pre_frame_update         () = 0;
    virtual void                    do_frame_update             () = 0;
    virtual void                    do_post_frame_update		() = 0;

    virtual size_2d                 do_main_window_size         () const = 0;


    virtual std::unique_ptr<system_window_base>  do_create_system_window        (size_2d size, std::string_view title) const = 0;

    // ---------------------------------
    // --- PRIVATE: Helper functions ---
    // ---------------------------------
    void                            process_events              ();

    // ----------------------------------
    // --- PRIVATE: Memeber variables ---
    // ----------------------------------
    std::string               app_name_         {"MyApp"};
    std::string               company_name_     {""};
    std::string               window_title_     {"Hello CPAF GUI"};

};

} //END namespace cpaf::gui

