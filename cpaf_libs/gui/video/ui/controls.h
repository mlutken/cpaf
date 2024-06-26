#pragma once
#include <chrono>
#include <memory>
#include <cpaf_libs/gui/color.h>
#include <cpaf_libs/gui/video/config.h>

namespace cpaf::locale {
class translator;
}

namespace cpaf::gui::video {


class player;

class controls
{
public:
    controls() = delete;
    controls(player& parent_player, config& cfg);
    virtual ~controls() = default;

    void            render                  ();
    void            render_stream_state     ()                          { do_render_stream_state(); }
    void            on_player_size_changed  ();
    bool            visible                 () const                    { return visible_ || prevent_hiding_; }
    void            show                    (bool do_show)              { visible_ = do_show; }
    void            show                    ()                          { visible_ = true; }
    void            hide                    ()                          { visible_ = false; }


protected:
    player&                 player_;
    config&                 config_;
    bool                    visible_ = true;

    void                    prevent_hiding              (bool do_prevent) { prevent_hiding_ = do_prevent; }


    float                   buttons_relative_ypos       () const { return config_.float_val ("controls", "buttons_relative_ypos");  }
    float                   buttons_relative_x_dist     () const { return config_.float_val ("controls", "buttons_relative_x_dist");}
    float                   play_buttons_size           () const { return config_.float_val ("controls", "play_buttons_size");      }
    float                   menu_buttons_size           () const { return config_.float_val ("controls", "menu_buttons_size");      }
    color                   buttons_color               () const { return config_.color     ("controls", "buttons_color");          }
    color                   menu_close_button_color     () const { return config_.color     ("controls", "menu_close_button_color");}

    float                   slider_relative_ypos        () const { return config_.float_val ("controls", "slider_relative_ypos");   }
    int32_t                 slider_height               () const { return config_.int32     ("controls", "slider_height");          }
    std::string             slider_font_name            () const { return config_.str       ("controls", "slider_font_name");       }

    std::string             time_font_name              () const { return config_.str       ("controls", "time_font_name");         }
    int32_t                 time_font_size              () const { return config_.int32     ("controls", "time_font_size");         }
    color                   time_text_color             () const { return config_.color     ("controls", "time_text_color");        }

    std::string             stream_state_font_name      () const { return config_.str       ("controls", "stream_state_font_name"); }
    int32_t                 stream_state_font_size      () const { return config_.int32     ("controls", "stream_state_font_size"); }

    std::chrono::seconds    skip_time_small             () const { return config_.time_s   ("controls", "skip_time_small");        }


    cpaf::locale::translator&           tr              ();
    const cpaf::locale::translator&     tr              () const;


private:
    bool            prevent_hiding_      = false;
    virtual void    do_calc_geometry        () = 0;
    virtual void    do_render               () = 0;
    virtual void    do_render_stream_state  () = 0;
};

} // namespace cpaf::gui::video

/*

            "time_font_name": "abeezee_regular",
            "time_font_size": 13,
            "time_text_color": [0,1,1,1],
            "time_bg_color": [0,0,0,1],
            "time_border_color": [0,0,0,1],

            "slider_height": 8,
            "relative_ypos": 0.7,
            "skip_time_small": 15


*/
