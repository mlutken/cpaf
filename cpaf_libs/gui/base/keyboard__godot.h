#pragma once

#include <config/cpaf_platform_definitions.h>
#include <cpaf_libs/gui/base/keyboard_base.h>

#if (CPAF_GUI_SYSTEM_ID != CPAF_SYSTEM_ID_GODOT)
#error Only include cpaf_libs/gui/base/keyboard__godot.h when CPAF_GUI_SYSTEM_ID is CPAF_SYSTEM_ID_GODOT
#endif

namespace cpaf::gui {


class keyboard_platform : public keyboard_base {
public:

private:
    // --- Platform overrides ---

    // --- PRIVATE: Helpers  ---
};


} //END namespace cpaf::gui
