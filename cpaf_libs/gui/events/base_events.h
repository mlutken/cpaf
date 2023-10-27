#pragma once
#include <cstdint>
#include <chrono>
#include <string>

namespace cpaf::gui::events {

using time_point = std::chrono::steady_clock::time_point;

enum class pointer_type {
    unknown,         //!< Pointer is a real mouse (as opposed to a touch event generating mouse events)
    mouse,           //!< Pointer is a real mouse (as opposed to a touch event generating mouse events)
    touch,           //!< Pointer a touch event
    COUNT
};


enum class is_handled {
    no,            //!< Event was not handled
    yes            //!< Event was handled
};

enum class to_str_mode {
    normal,            //!< Normal to_string formatting
    verbose            //!< Verbose to_string formatting
};

std::string to_name     (pointer_type t);
std::string to_name     (is_handled handled);


/** @brief Base event
*/
struct base
{
    ~base() = default;
    virtual std::string name() const { return "base"; }
    virtual std::string to_string(to_str_mode /*mode*/ = to_str_mode::normal) const { return name(); }

    time_point      timestamp;

};


/** @brief No event
*/
struct none : public base
{
    std::string name() const override { return "none"; }
};


/** @brief Application events
*/
struct application : public base {
public:
    std::string name() const override { return "application"; }

    enum class type {
        unknown,                    //!<
        user_quit,                  //!< User-requested quit
        terminating,                //!< The application is being terminated by the OS. iOS in applicationWillTerminate(), Android in onDestroy()
        low_memory,                 //!< The application is low on memory, free memory if possible. iOS in applicationDidReceiveMemoryWarning(), Android in onLowMemory()
        will_enter_background,      //!< The application is about to enter the background. iOS in applicationWillResignActive(), Android in onPause()
        did_enter_background,       //!< The application did enter the background and may not get CPU for some time. iOS in applicationDidEnterBackground(), Android in onPause()
        will_enter_foreground,      //!< The application is about to enter the foreground. iOS in applicationWillEnterForeground(), Android in onResume()
        did_enter_foreground,       //!< The application is now interactive. iOS in applicationDidBecomeActive(), Android in onResume()
        COUNT
    };

    static std::string  to_name     (application::type t);
    std::string         to_string   (to_str_mode mode = to_str_mode::normal) const override;

    application::type   tp      = application::type::unknown;   //!< Window event type
};

/** @brief Window events
*/
struct window : public base {
public:
    std::string name() const override { return "window"; }
    enum class type {
        unknown,                    //!<
        shown,                      //!<
        hidden,                     //!<
        exposed,                    //!<
        move,                       //!<
        resized,                    //!< The window was resized (data in event.size)
        minimized,                  //!< The window was minimized
        maximized,                  //!< The window was maximized
        restored,                   //!<
        mouse_entered,              //!<
        mouse_left,                 //!<
        focus_gained,               //!< The window gained the focus (no data)
        focus_lost,                 //!< The window lost the focus (no data)
        close,                      //!< The window requested to be closed (no data)
        COUNT
    };

    static std::string  to_name     (window::type t);
    std::string         to_string   (to_str_mode mode = to_str_mode::normal) const override;

    window::type        tp      = window::type::unknown;    //!< Window event type
    std::int32_t        width   = 0;                        //!< New width, in pixels
    std::int32_t        height  = 0;                        //!< New height, in pixels
};

/** @brief Main window / screen events
*/
struct display : public base
{
    std::string name() const override { return "display"; }
    enum class orientation : std::uint8_t
    {
        unknown,        //!< Horizontal display orientation
        horizontal,     //!< Horizontal display orientation
        vertical        //!< Vertical display orientation
    };

    static std::string  to_name     (orientation orient);
    std::string         to_string   (to_str_mode mode = to_str_mode::normal) const override;

    std::int32_t            width       = 0;                                    //!< New width, in pixels
    std::int32_t            height      = 0;                                    //!< New height, in pixels
    display::orientation    orien       = display::orientation::horizontal;     //!< New main window orientation
};



} //END namespace cpaf::gui::events
