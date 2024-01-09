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

    static constexpr float slider_illegal_pos   {-1};

    ImVec2      play_pause_btn_pos_         {};
    ImVec2      video_back_btn_pos_         {};
    ImVec2      video_fwd_btn_pos_          {};
    ImVec2      video_slider_pos_           {};
    ImVec2      video_slider_size_          {};
    ImVec2      elapsed_time_pos_           {};
    ImVec2      remaining_time_pos_         {};
    float       video_slider_grab_width_    {20};
    float       slider_last_user_pos_seconds_       {slider_illegal_pos};
};

} // namespace cpaf::gui::video
