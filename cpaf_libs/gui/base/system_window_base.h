#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <cpaf_libs/gui/gui_types.h>


namespace cpaf::gui {


class system_window_base {
public:
    explicit system_window_base (const size_2d& size);
    system_window_base          (const size_2d& size, std::string_view title);

    std::string                 dbg_characteristics         () const;
    size_2d                     get_size                    ()  { return do_get_size(); }
    std::string                 get_title                   ()  { return do_get_title(); }

protected:
    // --- PROTECTED: Helper functions for derived classes ---

private:
    virtual size_2d             do_get_size                 () = 0;
    virtual std::string         do_get_title                () = 0;
};

} //END namespace cpaf::gui

