#pragma once
#include <chrono>
#include <memory>

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

private:

    virtual void    do_render() = 0;

    bool            visible_ = false;


};

} // namespace cpaf::gui::video
