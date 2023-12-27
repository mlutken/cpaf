#include "font_size.h"
#include <cpaf_libs/gui/system.h>
#include <cpaf_libs/gui/system_window.h>

namespace cpaf::gui {

uint32_t font_size::to_pixels_display(float size_in_points, int32_t display_index)
{
    return to_pixels(size_in_points, system::instance().dpi(display_index));
}

uint32_t font_size::to_pixels(float size_in_points, const system_window& window)
{
    return to_pixels_display(size_in_points, window.display_index());
}

uint32_t font_size::to_pixels(float size_in_points, float dpi)
{
    return size_in_points*(dpi/dpi_base)*font_size::platform_scale_factor;
}

font_size font_size::create_for_display (float size_in_points, int32_t display_index)
{
    return font_size(size_in_points, system::instance().dpi(display_index));
}

uint32_t font_size::in_pixels() const
{
    return size_in_points_*dpi_factor_*font_size::platform_scale_factor;
}

uint32_t font_size::in_pixels(float viewport_height) const
{
    return size_in_points_ * viewport_height;
}


// --------------------------
// --- Platform constants ---
// --------------------------

#if (CPAF_PLATFORM_TYPE_ID == CPAF_PLATFORM_TYPE_ID_DESKTOP)
const float font_size::platform_scale_factor = 1.0;
#elif (CPAF_PLATFORM_TYPE_ID == CPAF_PLATFORM_TYPE_ID_EMBEDDED)
const float font_size::platform_scale_factor = 1.0;
#elif (CPAF_PLATFORM_TYPE_ID == CPAF_PLATFORM_TYPE_ID_DESKTOP_WEB)
const float font_size::platform_scale_factor = 1.0;
#elif (CPAF_PLATFORM_TYPE_ID == CPAF_PLATFORM_TYPE_ID_MOBILE)
const float font_size::platform_scale_factor = 1.0;
#elif (CPAF_PLATFORM_TYPE_ID == CPAF_PLATFORM_TYPE_ID_TABLET)
const float font_size::platform_scale_factor = 1.0;
#elif (CPAF_PLATFORM_TYPE_ID == CPAF_PLATFORM_TYPE_ID_TABLET_WEB)
const float font_size::platform_scale_factor = 1.0;
#elif (CPAF_PLATFORM_TYPE_ID == CPAF_PLATFORM_TYPE_ID_MOBILE_WEB)
const float font_size::platform_scale_factor = 1.0;
#endif



} // namespace cpaf::gui
