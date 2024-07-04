#pragma once

#include <memory>
#include <string>
#include <vector>
#include <config/cpaf_platform_definitions.h>
#include <cpaf_libs/gui/base/app_base.h>
#include <cpaf_libs/gui/events/events__godot.h>

#if (CPAF_GUI_SYSTEM_ID != CPAF_SYSTEM_ID_GODOT)
#error Only include cpaf_libs/gui/base/app__godot.h when CPAF_GUI_SYSTEM_ID is CPAF_SYSTEM_ID_GODOT
#endif

namespace cpaf::gui {


// https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples

class app_platform : public app_base {
public:
    app_platform();
    virtual ~app_platform();

private:
    // --- Platform overrides ---
    void                            do_platform_start_run				() override;
    events::event                   do_platform_get_event               () const override;
    void                            do_platform_process_events          () override;
    void                            do_platform_pre_frame_update        () override;
    void                            do_platform_frame_update            () override;
    void                            do_platform_post_frame_update       () override;
    size_2d                         do_platform_main_window_size        () const override;
    system_window&                  do_main_window                      () override;
    std::shared_ptr<system_window>  do_main_window_shared               () const override;
    void                            do_add_fonts                        (const std::string& font_name, const std::vector<uint32_t>& sizes_in_points) override;
    void                            do_add_fonts                        (const std::string& font_name,
                                                                         float size_adjust_factor,
                                                                         uint32_t range_min,
                                                                         uint32_t range_max,
                                                                         const std::vector<uint32_t>& sizes_in_points) override;
    void                            do_add_fonts                        (const std::string& font_name,
                                                                         const std::string& merge_font_name,
                                                                         float merge_size_adjust_factor,
                                                                         uint32_t range_min,
                                                                         uint32_t range_max,
                                                                         const std::vector<uint32_t>& sizes_in_points) override;
    void                            do_set_default_font                 (const std::string& font_name, uint32_t size_points) override;

    std::unique_ptr<system_window>  do_create_system_window   (size_2d size, std::string_view title) const override;

    // --- PRIVATE: Helpers  ---
    void        initialize();

    // --- PRIVATE: Members ---
    std::shared_ptr<system_window>  main_window_ptr_    {nullptr};

    bool                            m_show_some_panel   {true};
    bool                            m_show_debug_panel  {false};

};


} //END namespace cpaf::gui
