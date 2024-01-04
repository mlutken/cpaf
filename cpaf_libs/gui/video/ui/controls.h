#pragma once
#include <chrono>
#include <memory>
#include <cpaf_libs/gui/color.h>

namespace cpaf::gui::video {

class player;

class controls
{
public:
    controls() = delete;
    explicit controls(player& parent_player);
    virtual ~controls() = default;

    void            render      ();
    bool            visible     () const { return visible_; }
    void            show        (bool do_show) { visible_ = do_show; }

protected:
    player&         player_;
    std::string     font_name_                  {"manrope"};
    color           text_color_                 {1,1,1,1};
    color           border_color_               {0,0,0,1};
    color           bg_color_                   {0,0,0,1};
    uint16_t        time_font_size_points_      {13};
    uint16_t        slider_font_size_points_    {18};
    uint16_t        fwd_back_font_size_points_  {38};
    float           relative_ypos_              {0.5};
//    float           btns_rel_size_              {0.1};


private:


    virtual void    do_render() = 0;

    bool            visible_ = true;


};

} // namespace cpaf::gui::video
