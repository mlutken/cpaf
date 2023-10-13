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

    std::string                     dbg_characteristics         () const;
    exit_status_t                   run                         () { do_run(); return exit_status_; }

    std::unique_ptr<system_window_base>  create_system_window   (size_2d size, std::string_view title) const {
        fmt::println("FIXMENM create_system_window () ");
        return do_create_system_window(size, title);
    };

protected:
    exit_status_t                   exit_status_                    = exit_status_t::success;
    std::unique_ptr<system_window_base>  main_window_                    = nullptr;
    // --- PROTECTED: Helper functions for derived classes ---

private:

    virtual void                    do_run						() = 0;
    virtual std::unique_ptr<system_window_base>  do_create_system_window        (size_2d size, std::string_view title) const = 0;

};

} //END namespace cpaf::gui

