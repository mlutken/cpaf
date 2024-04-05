#pragma once
#include <chrono>
#include <memory>
#include <cpaf_libs/gui/color.h>
#include <cpaf_libs/gui/video/config.h>

namespace cpaf::gui::video {


class player;

class controls
{
public:
    controls() = delete;
    controls(player& parent_player, config& cfg);
    virtual ~controls() = default;

    void            render                  ();
    void            on_player_size_changed  ();
    bool            visible                 () const                    { return visible_; }
    void            show                    (bool do_show)              { visible_ = do_show; }
    void            show                    ()                          { visible_ = true; }
    void            hide                    ()                          { visible_ = false; }


protected:
    player&                 player_;
    config&                 config_;
    bool                    visible_ = true;

    float                   buttons_relative_ypos   () const { return config_.float_val ("controls", "buttons_relative_ypos");  }
    float                   buttons_relative_x_dist () const { return config_.float_val ("controls", "buttons_relative_x_dist");}
    std::string             buttons_font_name       () const { return config_.str       ("controls", "buttons_font_name");      }
    float                   buttons_size            () const { return config_.float_val ("controls", "buttons_size");           }
    color                   buttons_text_color      () const { return config_.color     ("controls", "buttons_text_color");     }
    color                   buttons_color           () const { return config_.color     ("controls", "buttons_color");          }
    color                   buttons_border_color    () const { return config_.color     ("controls", "buttons_border_color");   }

    std::string             menu_font_name          () const { return config_.str       ("controls", "menu_font_name");         }
    int32_t                 menu_font_size          () const { return config_.int32     ("controls", "menu_font_size");         }
    color                   menu_text_color         () const { return config_.color     ("controls", "menu_text_color");        }
    color                   menu_bg_color           () const { return config_.color     ("controls", "menu_bg_color");          }
    color                   menu_border_color       () const { return config_.color     ("controls", "menu_border_color");      }

    float                   slider_relative_ypos    () const { return config_.float_val ("controls", "slider_relative_ypos");   }
    int32_t                 slider_height           () const { return config_.int32     ("controls", "slider_height");          }
    std::string             slider_font_name        () const { return config_.str       ("controls", "slider_font_name");       }
    int32_t                 slider_font_size        () const { return config_.int32     ("controls", "slider_font_size");       }
    color                   slider_color            () const { return config_.color     ("controls", "slider_color");           }
    color                   slider_bg_color         () const { return config_.color     ("controls", "slider_bg_color");        }
    color                   slider_border_color     () const { return config_.color     ("controls", "slider_border_color");    }

    std::string             time_font_name          () const { return config_.str       ("controls", "time_font_name");         }
    int32_t                 time_font_size          () const { return config_.int32     ("controls", "time_font_size");         }
    color                   time_text_color         () const { return config_.color     ("controls", "time_text_color");        }
    color                   time_bg_color           () const { return config_.color     ("controls", "time_bg_color");          }
    color                   time_border_color       () const { return config_.color     ("controls", "time_border_color");      }

    std::chrono::seconds    skip_time_small         () const { return config_.seconds   ("controls", "skip_time_small");        }

private:
    virtual void    do_calc_geometry    () = 0;
    virtual void    do_render           () = 0;



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
