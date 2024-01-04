#pragma once
#include <imguipp/imgui_rai.h>
#include <cpaf_libs/gui/video/ui/controls.h>

namespace cpaf::gui::video {

class controls_default : public controls
{
public:
    using controls::controls;	// "Import" constructors to scope

private:
    void    calc_geometry   ();
    void    do_render       () override;

    ImVec2      video_back_btn_pos_         {};
    ImVec2      video_fwd_btn_pos_          {};
    ImVec2      video_slider_pos_           {};
    ImVec2      video_slider_size_          {};
    ImVec2      elapsed_time_pos_           {};
    ImVec2      remaining_time_pos_         {};
    float       video_slider_grab_width_    {20};
};

} // namespace cpaf::gui::video
