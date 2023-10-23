#pragma once
#include <cstdint>
#include <chrono>

namespace cpaf::gui::events {

using time_point = std::chrono::steady_clock::time_point;

enum class pointer_type {
    mouse,           //!< Pointer is a real mouse (as opposed to a touch event generating mouse events)
    touch,           //!< Pointer a touch event
    COUNT
};

enum class is_handled {
    no,             //!< Event was not handled
    yes            //!< Event was handled
};


/** @brief Base event
*/
struct base
{
    time_point      timestamp;
};


/** @brief No event
*/
struct none : public base
{
};

// /* Application events */
// SDL_QUIT           = 0x100, /**< User-requested quit */

//     /* These application events have special meaning on iOS, see README-ios.md for details */
//     SDL_APP_TERMINATING,        /**< The application is being terminated by the OS
//                                      Called on iOS in applicationWillTerminate()
//                                      Called on Android in onDestroy()
//                                 */
//     SDL_APP_LOWMEMORY,          /**< The application is low on memory, free memory if possible.
//                                      Called on iOS in applicationDidReceiveMemoryWarning()
//                                      Called on Android in onLowMemory()
//                                 */
//     SDL_APP_WILLENTERBACKGROUND, /**< The application is about to enter the background
//                                      Called on iOS in applicationWillResignActive()
//                                      Called on Android in onPause()
//                                 */
//     SDL_APP_DIDENTERBACKGROUND, /**< The application did enter the background and may not get CPU for some time
//                                      Called on iOS in applicationDidEnterBackground()
//                                      Called on Android in onPause()
//                                 */
//     SDL_APP_WILLENTERFOREGROUND, /**< The application is about to enter the foreground
//                                      Called on iOS in applicationWillEnterForeground()
//                                      Called on Android in onResume()
//                                 */
//     SDL_APP_DIDENTERFOREGROUND, /**< The application is now interactive
//                                      Called on iOS in applicationDidBecomeActive()
//                                      Called on Android in onResume()


/** @brief Application events
*/
struct application : public base {
public:
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

    application::type   tp      = application::type::unknown;   //!< Window event type
};

/** @brief Window events
*/
struct window : public base {
public:
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

    window::type        tp      = window::type::unknown;    //!< Window event type
    std::int32_t        width   = 0;                        //!< New width, in pixels
    std::int32_t        height  = 0;                        //!< New height, in pixels
};

/** @brief Main window / screen events
*/
struct display : public base
{
    enum class orientation : std::uint8_t
    {
        horizontal,     //!< Horizontal display orientation
        vertical        //!< Vertical display orientation
    };

    std::int32_t            width       = 0;                                    //!< New width, in pixels
    std::int32_t            height      = 0;                                    //!< New height, in pixels
    display::orientation    orientation = display::orientation::horizontal;     //!< New main window orientation
};



} //END namespace cpaf::gui::events
