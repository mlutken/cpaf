#pragma once
#include <chrono>
#include <memory>
#include <cpaf_libs/gui/color.h>

namespace cpaf::gui::video {

class config;

class player;

class controls
{
public:
    controls() = delete;
    explicit controls(player& parent_player, config& cfg);
    virtual ~controls() = default;

    void            render      ();
    bool            visible     () const { return visible_; }
    void            show        (bool do_show) { visible_ = do_show; }

protected:
    player&                 player_;
    config&                 config_;
    std::string             font_name               () const;
    color                   text_color              () const;
    color                   border_color            () const;
    color                   background_color        () const;
    uint16_t                time_font_size   () const;
    uint16_t                slider_height           () const;
    uint16_t                buttons_size            () const;
    float                   relative_ypos           () const;

private:


    virtual void    do_render() = 0;

    bool            visible_ = true;


};

} // namespace cpaf::gui::video
