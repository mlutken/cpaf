#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <cpaf_libs/gui/gui_types.h>


namespace cpaf::gui {


class app_base {
public:

    std::string         dbg_characteristics     () const;

protected:
    // --- PROTECTED: Helper functions for derived classes ---

private:

};

} //END namespace cpaf::gui

