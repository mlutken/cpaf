#pragma once

#include <cstdint>
#include <string>
#include <cpaf_libs/gui/events/base_events.h>

namespace cpaf::gui::events {


struct keyboard : public base {

    enum class type {
        unknown,                //!< Unknown type
        pressed,                //!< A key was pressed
        released,               //!< A key was released
        character,              //!< A character was entered (data in u8char / u32char)
        text,                   //!< A string of text was entered (data in text / u32char)
        COUNT
    };

    enum class key: std::int32_t
    {
        unknown = -1, //!< Unhandled key
        a       = 0,  //!< The A key
        b,            //!< The B key
        c,            //!< The C key
        d,            //!< The D key
        e,            //!< The E key
        f,            //!< The F key
        g,            //!< The G key
        h,            //!< The H key
        i,            //!< The I key
        j,            //!< The J key
        k,            //!< The K key
        l,            //!< The L key
        m,            //!< The M key
        n,            //!< The N key
        o,            //!< The O key
        p,            //!< The P key
        q,            //!< The Q key
        r,            //!< The R key
        s,            //!< The S key
        t,            //!< The T key
        u,            //!< The U key
        v,            //!< The V key
        w,            //!< The W key
        x,            //!< The X key
        y,            //!< The Y key
        z,            //!< The Z key
        num0,         //!< The 0 key
        num1,         //!< The 1 key
        num2,         //!< The 2 key
        num3,         //!< The 3 key
        num4,         //!< The 4 key
        num5,         //!< The 5 key
        num6,         //!< The 6 key
        num7,         //!< The 7 key
        num8,         //!< The 8 key
        num9,         //!< The 9 key
        escape,       //!< The Escape key
        lcontrol,     //!< The left Control key
        lshift,       //!< The left Shift key
        lalt,         //!< The left Alt key
        lsystem,      //!< The left OS specific key: window (Windows and Linux), apple (macOS), ...
        rcontrol,     //!< The right Control key
        rshift,       //!< The right Shift key
        ralt,         //!< The right Alt key
        rsystem,      //!< The right OS specific key: window (Windows and Linux), apple (macOS), ...
        menu,         //!< The Menu key
        lbracket,     //!< The [ key
        rbracket,     //!< The ] key
        semicolon,    //!< The ; key
        comma,        //!< The , key
        period,       //!< The . key
        apostrophe,   //!< The ' key
        slash,        //!< The / key
        backslash,    //!< The \ key
        grave,        //!< The ` key
        equal,        //!< The = key
        hyphen,       //!< The - key (hyphen)
        space,        //!< The Space key
        enter,        //!< The Enter/Return keys
        backspace,    //!< The Backspace key
        tab,          //!< The Tabulation key
        pageup,       //!< The Page up key
        pagedown,     //!< The Page down key
        end,          //!< The End key
        home,         //!< The Home key
        insert,       //!< The Insert key
        del,          //!< The Delete key
        ddd,          //!< The + key
        subtract,     //!< The - key (minus, usually from numpad)
        multiply,     //!< The * key
        divide,       //!< The / key
        left,         //!< Left arrow
        right,        //!< Right arrow
        up,           //!< Up arrow
        down,         //!< Down arrow
        numpad0,      //!< The numpad 0 key
        numpad1,      //!< The numpad 1 key
        numpad2,      //!< The numpad 2 key
        numpad3,      //!< The numpad 3 key
        numpad4,      //!< The numpad 4 key
        numpad5,      //!< The numpad 5 key
        numpad6,      //!< The numpad 6 key
        numpad7,      //!< The numpad 7 key
        numpad8,      //!< The numpad 8 key
        numpad9,      //!< The numpad 9 key
        f1,           //!< The F1 key
        f2,           //!< The F2 key
        f3,           //!< The F3 key
        f4,           //!< The F4 key
        f5,           //!< The F5 key
        f6,           //!< The F6 key
        f7,           //!< The F7 key
        f8,           //!< The F8 key
        f9,           //!< The F9 key
        f10,          //!< The F10 key
        f11,          //!< The F11 key
        f12,          //!< The F12 key
        f13,          //!< The F13 key
        f14,          //!< The F14 key
        f15,          //!< The F15 key
        pause,        //!< The Pause key

        COUNT, //!< Keep last -- the total number of keyboard keys
    };

    ////////////////////////////////////////////////////////////
    /// \brief Scancodes
    ///
    /// The enumerators are bound to a physical key and do not depend on
    /// the keyboard layout used by the operating system. Usually, the AT-101
    /// keyboard can be used as reference for the physical position of the keys.
    ///
    ////////////////////////////////////////////////////////////
    enum class scan_code : std::int32_t
    {
        unknown = -1, //!< Represents any scancode not present in this enum
        a       = 0,  //!< Keyboard a and A key
        b,            //!< Keyboard b and B key
        c,            //!< Keyboard c and C key
        d,            //!< Keyboard d and D key
        e,            //!< Keyboard e and E key
        f,            //!< Keyboard f and F key
        g,            //!< Keyboard g and G key
        h,            //!< Keyboard h and H key
        i,            //!< Keyboard i and I key
        j,            //!< Keyboard j and J key
        k,            //!< Keyboard k and K key
        l,            //!< Keyboard l and L key
        m,            //!< Keyboard m and M key
        n,            //!< Keyboard n and N key
        o,            //!< Keyboard o and O key
        p,            //!< Keyboard p and P key
        q,            //!< Keyboard q and Q key
        r,            //!< Keyboard r and R key
        s,            //!< Keyboard s and S key
        t,            //!< Keyboard t and T key
        u,            //!< Keyboard u and U key
        v,            //!< Keyboard v and V key
        w,            //!< Keyboard w and W key
        x,            //!< Keyboard x and X key
        y,            //!< Keyboard y and Y key
        z,            //!< Keyboard z and Z key
        num1,         //!< Keyboard 1 and ! key
        num2,         //!< Keyboard 2 and @ key
        num3,         //!< Keyboard 3 and # key
        num4,         //!< Keyboard 4 and $ key
        num5,         //!< Keyboard 5 and % key
        num6,         //!< Keyboard 6 and ^ key
        num7,         //!< Keyboard 7 and & key
        num8,         //!< Keyboard 8 and * key
        num9,         //!< Keyboard 9 and ) key
        num0,         //!< Keyboard 0 and ) key
        enter,        //!< Keyboard Enter/Return key
        escape,       //!< Keyboard Escape key
        backspace,    //!< Keyboard Backspace key
        tab,          //!< Keyboard Tab key
        space,        //!< Keyboard Space key
        hyphen,       //!< Keyboard - and _ key
        equal,        //!< Keyboard = and +
        lbracket,     //!< Keyboard [ and { key
        rbracket,     //!< Keyboard ] and } key
        // For US keyboards mapped to key 29 (Microsoft Keyboard Scan Code Specification)
        // For Non-US keyboards mapped to key 42 (Microsoft Keyboard Scan Code Specification)
        // Typical language mappings: Belg:£µ` FrCa:<>} Dan:*' Dutch:`´ Fren:µ* Ger:'# Ital:§ù LatAm:[}` Nor:*@ Span:ç} Swed:*' Swiss:$£} UK:~# Brazil:}]
        backslash,      //!< Keyboard \ and | key OR various keys for Non-US keyboards
        semicolon,      //!< Keyboard ; and : key
        apostrophe,     //!< Keyboard ' and " key
        grave,          //!< Keyboard ` and ~ key
        comma,          //!< Keyboard , and < key
        period,         //!< Keyboard . and > key
        slash,          //!< Keyboard / and ? key
        f1,             //!< Keyboard F1 key
        f2,             //!< Keyboard F2 key
        f3,             //!< Keyboard F3 key
        f4,             //!< Keyboard F4 key
        f5,             //!< Keyboard F5 key
        f6,             //!< Keyboard F6 key
        f7,             //!< Keyboard F7 key
        f8,             //!< Keyboard F8 key
        f9,             //!< Keyboard F9 key
        f10,            //!< Keyboard F10 key
        f11,            //!< Keyboard F11 key
        f12,            //!< Keyboard F12 key
        f13,            //!< Keyboard F13 key
        f14,            //!< Keyboard F14 key
        f15,            //!< Keyboard F15 key
        f16,            //!< Keyboard F16 key
        f17,            //!< Keyboard F17 key
        f18,            //!< Keyboard F18 key
        f19,            //!< Keyboard F19 key
        f20,            //!< Keyboard F20 key
        f21,            //!< Keyboard F21 key
        f22,            //!< Keyboard F22 key
        f23,            //!< Keyboard F23 key
        f24,            //!< Keyboard F24 key
        capslock,       //!< Keyboard Caps %Lock key
        printscreen,    //!< Keyboard Print Screen key
        scrolllock,     //!< Keyboard Scroll %Lock key
        pause,          //!< Keyboard Pause key
        insert,         //!< Keyboard Insert key
        home,           //!< Keyboard Home key
        pageup,         //!< Keyboard Page Up key
        del,            //!< Keyboard Delete Forward key
        end,            //!< Keyboard End key
        pagedown,       //!< Keyboard Page Down key
        right,          //!< Keyboard Right Arrow key
        left,           //!< Keyboard Left Arrow key
        down,           //!< Keyboard Down Arrow key
        up,             //!< Keyboard Up Arrow key
        numlock,        //!< Keypad Num %Lock and Clear key
        numpaddivide,   //!< Keypad / key
        numpadmultiply, //!< Keypad * key
        numpadminus,    //!< Keypad - key
        numpadplus,     //!< Keypad + key
        numpadequal,    //!< keypad = key
        numpadenter,    //!< Keypad Enter/Return key
        numpaddecimal,  //!< Keypad . and Delete key
        numpad1,        //!< Keypad 1 and End key
        numpad2,        //!< Keypad 2 and Down Arrow key
        numpad3,        //!< Keypad 3 and Page Down key
        numpad4,        //!< Keypad 4 and Left Arrow key
        numpad5,        //!< Keypad 5 key
        numpad6,        //!< Keypad 6 and Right Arrow key
        numpad7,        //!< Keypad 7 and Home key
        numpad8,        //!< Keypad 8 and Up Arrow key
        numpad9,        //!< Keypad 9 and Page Up key
        numpad0,        //!< Keypad 0 and Insert key
        // For US keyboards doesn't exist
        // For Non-US keyboards mapped to key 45 (Microsoft Keyboard Scan Code Specification)
        // Typical language mappings: Belg:<\> FrCa:«°» Dan:<\> Dutch:]|[ Fren:<> Ger:<|> Ital:<> LatAm:<> Nor:<> Span:<> Swed:<|> Swiss:<\> UK:\| Brazil: \|.
        nonusbackslash,     //!< Keyboard Non-US \ and | key
        application,        //!< Keyboard Application key
        execute,            //!< Keyboard Execute key
        modechange,         //!< Keyboard Mode Change key
        help,               //!< Keyboard Help key
        menu,               //!< Keyboard Menu key
        select,             //!< Keyboard Select key
        redo,               //!< Keyboard Redo key
        undo,               //!< Keyboard Undo key
        cut,                //!< Keyboard Cut key
        copy,               //!< Keyboard Copy key
        paste,              //!< Keyboard Paste key
        volumemute,         //!< Keyboard Volume Mute key
        volumeup,           //!< Keyboard Volume Up key
        volumedown,         //!< Keyboard Volume Down key
        mediaplaypause,     //!< Keyboard Media Play Pause key
        mediastop,          //!< Keyboard Media Stop key
        medianexttrack,     //!< Keyboard Media Next Track key
        mediaprevioustrack, //!< Keyboard Media Previous Track key
        lcontrol,           //!< Keyboard Left Control key
        lshift,             //!< Keyboard Left Shift key
        lalt,               //!< Keyboard Left Alt key
        lsystem,            //!< Keyboard Left System key
        rcontrol,           //!< Keyboard Right Control key
        rshift,             //!< Keyboard Right Shift key
        ralt,               //!< Keyboard Right Alt key
        rsystem,            //!< Keyboard Right System key
        back,               //!< Keyboard Back key
        forward,            //!< Keyboard Forward key
        refresh,            //!< Keyboard Refresh key
        stop,               //!< Keyboard Stop key
        search,             //!< Keyboard Search key
        favorites,          //!< Keyboard Favorites key
        homepage,           //!< Keyboard Home Page key
        launchapplication1, //!< Keyboard Launch Application 1 key
        launchapplication2, //!< Keyboard Launch Application 2 key
        launchmail,         //!< Keyboard Launch Mail key
        launchmediaselect,  //!< Keyboard Launch Media Select key

        COUNT //!< Keep last -- the total number of scancodes
    };

    static std::string  to_name (keyboard::type t);
    static std::string  to_name (keyboard::key k);
    static std::string  to_name (keyboard::scan_code sc);

    std::string         name        () const override { return "keyboard"; }
    std::string         to_string   (to_str_mode mode = to_str_mode::normal) const override;

    keyboard::type          tp          = keyboard::type::pressed;      //!< Keyboard event type
    keyboard::key           k           = keyboard::key::unknown;       //!< Key
    keyboard::scan_code     sc          = keyboard::scan_code::unknown; //!< Scancode
    uint32_t                u8char      = 0;                            //!< UTF-8 character code
    // uint32_t                u32char; // TODO: Perhaps ....           //!< UTF-32 character code
    std::string             text          {};                           //!< Text thas was entered/edited
    bool                    alt         = false;                        //!< Is the Alt key pressed?
    bool                    control     = false;                        //!< Is the Control key pressed?
    bool                    shift       = false;                        //!< Is the Shift key pressed?
    bool                    system      = false;                        //!< Is the System key pressed?
};


} //END namespace cpaf::gui::events
