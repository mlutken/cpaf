#include "keyboard_events.h"
#include <fmt/format.h>

namespace cpaf::gui::events {

std::string events::keyboard::to_name(type t)
{
    switch (t) {
    case keyboard::type::pressed        : return "pressed";     break;
    case keyboard::type::released       : return "released";    break;
    case keyboard::type::character      : return "character";   break;
    case keyboard::type::text_input     : return "text_input";  break;
    case keyboard::type::text_editing   : return "text_editing";  break;

    default:
        break;
    }
    return "unknown";
}

std::string events::keyboard::to_name(key k)
{
    switch (k) {
    case keyboard::key::unknown     : return "unknown";           break;
    case keyboard::key::a           : return "a";           break;
    case keyboard::key::b           : return "b";           break;
    case keyboard::key::c           : return "c";           break;
    case keyboard::key::d           : return "d";           break;
    case keyboard::key::e           : return "e";           break;
    case keyboard::key::f           : return "f";           break;
    case keyboard::key::g           : return "g";           break;
    case keyboard::key::h           : return "h";           break;
    case keyboard::key::i           : return "i";           break;
    case keyboard::key::j           : return "j";           break;
    case keyboard::key::k           : return "k";           break;
    case keyboard::key::l           : return "l";           break;
    case keyboard::key::m           : return "m";           break;
    case keyboard::key::n           : return "n";           break;
    case keyboard::key::o           : return "o";           break;
    case keyboard::key::p           : return "p";           break;
    case keyboard::key::q           : return "q";           break;
    case keyboard::key::r           : return "r";           break;
    case keyboard::key::s           : return "s";           break;
    case keyboard::key::t           : return "t";           break;
    case keyboard::key::u           : return "u";           break;
    case keyboard::key::v           : return "v";           break;
    case keyboard::key::w           : return "w";           break;
    case keyboard::key::x           : return "x";           break;
    case keyboard::key::y           : return "y";           break;
    case keyboard::key::z           : return "z";           break;
    case keyboard::key::num0        : return "num0";        break;
    case keyboard::key::num1        : return "num1";        break;
    case keyboard::key::num2        : return "num2";        break;
    case keyboard::key::num3        : return "num3";        break;
    case keyboard::key::num4        : return "num4";        break;
    case keyboard::key::num5        : return "num5";        break;
    case keyboard::key::num6        : return "num6";        break;
    case keyboard::key::num7        : return "num7";        break;
    case keyboard::key::num8        : return "num8";        break;
    case keyboard::key::num9        : return "num9";        break;
    case keyboard::key::escape      : return "escape";      break;
    case keyboard::key::lcontrol    : return "lcontrol";	break;
    case keyboard::key::lshift      : return "lshift";      break;
    case keyboard::key::lalt        : return "lalt";        break;
    case keyboard::key::lsystem     : return "lsystem";     break;
    case keyboard::key::rcontrol    : return "rcontrol";	break;
    case keyboard::key::rshift      : return "rshift";      break;
    case keyboard::key::ralt        : return "ralt";        break;
    case keyboard::key::rsystem     : return "rsystem";     break;
    case keyboard::key::menu        : return "menu";        break;
    case keyboard::key::lbracket    : return "lbracket";	break;
    case keyboard::key::rbracket    : return "rbracket";	break;
    case keyboard::key::at          : return "at";          break;
    case keyboard::key::underscore  : return "underscore";	break;
    case keyboard::key::hash        : return "hash";        break;
    case keyboard::key::percent     : return "percent";     break;
    case keyboard::key::dollar      : return "dollar";      break;
    case keyboard::key::ampersand   : return "ampersand";	break;
    case keyboard::key::quote       : return "quote";       break;
    case keyboard::key::backquote   : return "backquote";	break;
    case keyboard::key::lparen      : return "lparen";      break;
    case keyboard::key::rparen      : return "rparen";      break;
    case keyboard::key::asterisk    : return "asterisk";	break;
    case keyboard::key::plus        : return "plus";        break;
    case keyboard::key::minus       : return "minus";       break;
    case keyboard::key::capslock    : return "capslock";	break;
    case keyboard::key::printscreen : return "printscreen";	break;
    case keyboard::key::scrolllock  : return "scrolllock";	break;
    case keyboard::key::colon       : return "colon";       break;
    case keyboard::key::semicolon   : return "semicolon";	break;
    case keyboard::key::less        : return "less";        break;
    case keyboard::key::greater     : return "greater";     break;
    case keyboard::key::question    : return "question";    break;
    case keyboard::key::comma       : return "comma";       break;
    case keyboard::key::period      : return "period";      break;
    case keyboard::key::apostrophe  : return "apostrophe";  break;
    case keyboard::key::slash       : return "slash";       break;
    case keyboard::key::backslash   : return "backslash";	break;
    case keyboard::key::grave       : return "grave";       break;
    case keyboard::key::equals      : return "equals";      break;
    case keyboard::key::hyphen      : return "hyphen";      break;
    case keyboard::key::space       : return "space";       break;
    case keyboard::key::enter       : return "enter";       break;
    case keyboard::key::backspace   : return "backspace";	break;
    case keyboard::key::tab         : return "tab";         break;
    case keyboard::key::pageup      : return "pageup";      break;
    case keyboard::key::pagedown    : return "pagedown";	break;
    case keyboard::key::end         : return "end";         break;
    case keyboard::key::home        : return "home";        break;
    case keyboard::key::insert      : return "insert";      break;
    case keyboard::key::del         : return "del";         break;
    case keyboard::key::numpad_plus : return "numpad_plus"; break;
    case keyboard::key::numpad_minus: return "numpad_minus";break;
    case keyboard::key::numpad_multiply : return "numpad_multiply";	break;
    case keyboard::key::numpad_divide: return "numpad_divide";      break;
    case keyboard::key::numpad_enter: return "numpad_enter";      break;
    case keyboard::key::numpad_period:return "numpad_period";      break;
    case keyboard::key::numpad_comma: return "numpad_comma";      break;
    case keyboard::key::numpad_equals: return "numpad_equals";      break;
    case keyboard::key::left        : return "left";        break;
    case keyboard::key::right       : return "right";       break;
    case keyboard::key::up          : return "up";          break;
    case keyboard::key::down        : return "down";        break;
    case keyboard::key::numpad0     : return "numpad0";     break;
    case keyboard::key::numpad1     : return "numpad1";     break;
    case keyboard::key::numpad2     : return "numpad2";     break;
    case keyboard::key::numpad3     : return "numpad3";     break;
    case keyboard::key::numpad4     : return "numpad4";     break;
    case keyboard::key::numpad5     : return "numpad5";     break;
    case keyboard::key::numpad6     : return "numpad6";     break;
    case keyboard::key::numpad7     : return "numpad7";     break;
    case keyboard::key::numpad8     : return "numpad8";     break;
    case keyboard::key::numpad9     : return "numpad9";     break;
    case keyboard::key::f1          : return "f1";          break;
    case keyboard::key::f2          : return "f2";          break;
    case keyboard::key::f3          : return "f3";          break;
    case keyboard::key::f4          : return "f4";          break;
    case keyboard::key::f5          : return "f5";          break;
    case keyboard::key::f6          : return "f6";          break;
    case keyboard::key::f7          : return "f7";          break;
    case keyboard::key::f8          : return "f8";          break;
    case keyboard::key::f9          : return "f9";          break;
    case keyboard::key::f10         : return "f10";         break;
    case keyboard::key::f11         : return "f11";         break;
    case keyboard::key::f12         : return "f12";         break;
    case keyboard::key::f13         : return "f13";         break;
    case keyboard::key::f14         : return "f14";         break;
    case keyboard::key::f15         : return "f15";         break;
    case keyboard::key::f16         : return "f16";         break;
    case keyboard::key::f17         : return "f17";         break;
    case keyboard::key::f18         : return "f18";         break;
    case keyboard::key::f19         : return "f19";         break;
    case keyboard::key::f20         : return "f20";         break;
    case keyboard::key::f21         : return "f21";         break;
    case keyboard::key::f22         : return "f22";         break;
    case keyboard::key::f23         : return "f23";         break;
    case keyboard::key::f24         : return "f24";         break;
    case keyboard::key::pause       : return "pause";       break;

    case keyboard::key::COUNT       : return "unknown";     break;
    }
    return "unknown";
}

std::string events::keyboard::to_name(scan_code sc)
{
    switch (sc) {
    case keyboard::scan_code::unknown	        : return "unknown";             break;
    case keyboard::scan_code::a	                : return "a";	                break;
    case keyboard::scan_code::b	                : return "b";	                break;
    case keyboard::scan_code::c	                : return "c";	                break;
    case keyboard::scan_code::d	                : return "d";	                break;
    case keyboard::scan_code::e	                : return "e";	                break;
    case keyboard::scan_code::f	                : return "f";	                break;
    case keyboard::scan_code::g	                : return "g";	                break;
    case keyboard::scan_code::h	                : return "h";	                break;
    case keyboard::scan_code::i	                : return "i";	                break;
    case keyboard::scan_code::j	                : return "j";	                break;
    case keyboard::scan_code::k	                : return "k";	                break;
    case keyboard::scan_code::l	                : return "l";	                break;
    case keyboard::scan_code::m	                : return "m";	                break;
    case keyboard::scan_code::n	                : return "n";	                break;
    case keyboard::scan_code::o	                : return "o";	                break;
    case keyboard::scan_code::p	                : return "p";	                break;
    case keyboard::scan_code::q	                : return "q";	                break;
    case keyboard::scan_code::r	                : return "r";	                break;
    case keyboard::scan_code::s	                : return "s";	                break;
    case keyboard::scan_code::t	                : return "t";	                break;
    case keyboard::scan_code::u	                : return "u";	                break;
    case keyboard::scan_code::v	                : return "v";	                break;
    case keyboard::scan_code::w                 : return "w";	                break;
    case keyboard::scan_code::x                 : return "x";                	break;
    case keyboard::scan_code::y                 : return "y";	                break;
    case keyboard::scan_code::z                 : return "z";                   break;
    case keyboard::scan_code::num1              : return "num1";                break;
    case keyboard::scan_code::num2              : return "num2";                break;
    case keyboard::scan_code::num3              : return "num3";                break;
    case keyboard::scan_code::num4              : return "num4";                break;
    case keyboard::scan_code::num5              : return "num5";                break;
    case keyboard::scan_code::num6              : return "num6";                break;
    case keyboard::scan_code::num7              : return "num7";                break;
    case keyboard::scan_code::num8              : return "num8";                break;
    case keyboard::scan_code::num9              : return "num9";                break;
    case keyboard::scan_code::num0              : return "num0";                break;
    case keyboard::scan_code::enter             : return "enter";               break;
    case keyboard::scan_code::escape            : return "escape";              break;
    case keyboard::scan_code::backspace         : return "backspace";           break;
    case keyboard::scan_code::tab               : return "tab";                 break;
    case keyboard::scan_code::space             : return "space";               break;
    case keyboard::scan_code::hyphen            : return "hyphen";              break;
    case keyboard::scan_code::equals            : return "equals";              break;
    case keyboard::scan_code::lbracket          : return "lbracket";            break;
    case keyboard::scan_code::rbracket          : return "rbracket";            break;
    case keyboard::scan_code::backslash         : return "backslash";           break;
    case keyboard::scan_code::semicolon         : return "semicolon";           break;
    case keyboard::scan_code::apostrophe        : return "apostrophe";          break;
    case keyboard::scan_code::grave             : return "grave";               break;
    case keyboard::scan_code::comma             : return "comma";               break;
    case keyboard::scan_code::period            : return "period";              break;
    case keyboard::scan_code::slash             : return "slash";               break;
    case keyboard::scan_code::f1                : return "f1";	                break;
    case keyboard::scan_code::f2                : return "f2";	                break;
    case keyboard::scan_code::f3                : return "f3";	                break;
    case keyboard::scan_code::f4                : return "f4";	                break;
    case keyboard::scan_code::f5                : return "f5";	                break;
    case keyboard::scan_code::f6                : return "f6";	                break;
    case keyboard::scan_code::f7                : return "f7";	                break;
    case keyboard::scan_code::f8                : return "f8";	                break;
    case keyboard::scan_code::f9                : return "f9";	                break;
    case keyboard::scan_code::f10               : return "f10";	                break;
    case keyboard::scan_code::f11               : return "f11";	                break;
    case keyboard::scan_code::f12               : return "f12";	                break;
    case keyboard::scan_code::f13               : return "f13";	                break;
    case keyboard::scan_code::f14               : return "f14";	                break;
    case keyboard::scan_code::f15               : return "f15";	                break;
    case keyboard::scan_code::f16               : return "f16";	                break;
    case keyboard::scan_code::f17               : return "f17";	                break;
    case keyboard::scan_code::f18               : return "f18";	                break;
    case keyboard::scan_code::f19               : return "f19";	                break;
    case keyboard::scan_code::f20               : return "f20";	                break;
    case keyboard::scan_code::f21               : return "f21";	                break;
    case keyboard::scan_code::f22               : return "f22";	                break;
    case keyboard::scan_code::f23               : return "f23";                 break;
    case keyboard::scan_code::f24               : return "f24";                 break;
    case keyboard::scan_code::capslock          : return "capslock";            break;
    case keyboard::scan_code::printscreen       : return "printscreen";         break;
    case keyboard::scan_code::scrolllock        : return "scrolllock";          break;
    case keyboard::scan_code::pause             : return "pause";               break;
    case keyboard::scan_code::insert            : return "insert";              break;
    case keyboard::scan_code::home              : return "home";                break;
    case keyboard::scan_code::pageup            : return "pageup";              break;
    case keyboard::scan_code::del               : return "del";                 break;
    case keyboard::scan_code::end               : return "end";                 break;
    case keyboard::scan_code::pagedown          : return "pagedown";            break;
    case keyboard::scan_code::right             : return "right";               break;
    case keyboard::scan_code::left              : return "left";                break;
    case keyboard::scan_code::down              : return "down";                break;
    case keyboard::scan_code::up                : return "up";                  break;
    case keyboard::scan_code::numlockclear      : return "numlockclear";        break;
    case keyboard::scan_code::numpaddivide      : return "numpaddivide";        break;
    case keyboard::scan_code::numpadmultiply    : return "numpadmultiply";      break;
    case keyboard::scan_code::numpadminus       : return "numpadminus";         break;
    case keyboard::scan_code::numpadplus        : return "numpadplus";          break;
    case keyboard::scan_code::numpadequal       : return "numpadequal";         break;
    case keyboard::scan_code::numpadenter       : return "numpadenter";         break;
    case keyboard::scan_code::numpaddecimaldel  : return "numpaddecimaldel";    break;
    case keyboard::scan_code::numpad1           : return "numpad1";             break;
    case keyboard::scan_code::numpad2           : return "numpad2";             break;
    case keyboard::scan_code::numpad3           : return "numpad3";             break;
    case keyboard::scan_code::numpad4           : return "numpad4";             break;
    case keyboard::scan_code::numpad5           : return "numpad5";             break;
    case keyboard::scan_code::numpad6           : return "numpad6";             break;
    case keyboard::scan_code::numpad7           : return "numpad7";             break;
    case keyboard::scan_code::numpad8           : return "numpad8";             break;
    case keyboard::scan_code::numpad9           : return "numpad9";             break;
    case keyboard::scan_code::numpad0           : return "numpad0";             break;
    case keyboard::scan_code::nonusbackslash    : return "nonusbackslash";      break;
    case keyboard::scan_code::application       : return "application";         break;
    case keyboard::scan_code::execute           : return "execute";             break;
    case keyboard::scan_code::modechange        : return "modechange";          break;
    case keyboard::scan_code::help              : return "help";                break;
    case keyboard::scan_code::menu              : return "menu";                break;
    case keyboard::scan_code::select            : return "select";              break;
    case keyboard::scan_code::redo              : return "redo";                break;
    case keyboard::scan_code::undo              : return "undo";                break;
    case keyboard::scan_code::cut               : return "cut";                 break;
    case keyboard::scan_code::copy              : return "copy";                break;
    case keyboard::scan_code::paste             : return "paste";               break;
    case keyboard::scan_code::volumemute        : return "volumemute";          break;
    case keyboard::scan_code::volumeup          : return "volumeup";            break;
    case keyboard::scan_code::volumedown        : return "volumedown";          break;
    case keyboard::scan_code::mediaplaypause    : return "mediaplaypause";      break;
    case keyboard::scan_code::mediastop         : return "mediastop";           break;
    case keyboard::scan_code::medianext         : return "medianext";           break;
    case keyboard::scan_code::mediaprevious     : return "mediaprevious";       break;
    case keyboard::scan_code::lcontrol          : return "lcontrol";            break;
    case keyboard::scan_code::lshift            : return "lshift";              break;
    case keyboard::scan_code::lalt              : return "lalt";                break;
    case keyboard::scan_code::lsystem           : return "lsystem";             break;
    case keyboard::scan_code::rcontrol          : return "rcontrol";            break;
    case keyboard::scan_code::rshift            : return "rshift";              break;
    case keyboard::scan_code::ralt              : return "ralt";                break;
    case keyboard::scan_code::rsystem           : return "rsystem";             break;
    case keyboard::scan_code::back              : return "back";                break;
    case keyboard::scan_code::forward           : return "forward";             break;
    case keyboard::scan_code::refresh           : return "refresh";             break;
    case keyboard::scan_code::stop              : return "stop";                break;
    case keyboard::scan_code::search            : return "search";              break;
    case keyboard::scan_code::favorites         : return "favorites";           break;
    case keyboard::scan_code::homepage          : return "homepage";            break;
    case keyboard::scan_code::launchapplication1: return "launchapplication1";	break;
    case keyboard::scan_code::launchapplication2: return "launchapplication2";	break;
    case keyboard::scan_code::launchmail        : return "launchmail";          break;
    case keyboard::scan_code::launchmediaselect	: return "launchmediaselect";	break;
    case keyboard::scan_code::COUNT             : return "unknown";     break;
    }
    return "unknown";
}

std::string keyboard::to_string(to_str_mode /*mode*/) const
{
    auto s = name() + "|" + keyboard::to_name(tp);
    switch (tp) {
    case keyboard::type::pressed:
    case keyboard::type::released:
    case keyboard::type::character:
        s += fmt::format(" [key: {} scan code: {}]", to_name(k), to_name(sc));
        break;
    case keyboard::type::text_input:
        s += fmt::format(" [text: {}]", text);
    default:
        break;
    }
    return s;
}

} //END namespace cpaf::gui::events
