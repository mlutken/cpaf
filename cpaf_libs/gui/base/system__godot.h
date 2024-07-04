#pragma once
#include <config/cpaf_platform_definitions.h>
#include <cpaf_libs/gui/base/system_base.h>

#if (CPAF_GUI_SYSTEM_ID != CPAF_SYSTEM_ID_GODOT)
#error Only include cpaf_libs/gui/base/system__godot.h when CPAF_GUI_SYSTEM_ID is CPAF_SYSTEM_ID_GODOT
#endif

namespace cpaf::gui {


class system_platform : public system_base {
public:

    ~system_platform();

private:
	// --- Platform overrides ---
    int32_t			do_init						() override;
    int32_t         do_display_count			() const override;
    float			do_dpi						(int32_t display_index) const override;

    // --- PRIVATE: Helpers  ---
};


} //END namespace cpaf::gui



