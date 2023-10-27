#include "event.h"
#include <variant>

namespace cpaf::gui::events {

// helper constant for the visitor #3
template<class>
inline constexpr bool always_false_v = false;

std::string event::name() const
{
    std::string n = std::visit([this](auto&& arg)
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr        (std::is_same_v<T, events::none>) { return std::get<T>(instance).name(); }
        else if constexpr   (std::is_same_v<T, events::application>) { return std::get<T>(instance).name(); }
        else if constexpr   (std::is_same_v<T, events::window>) { return std::get<T>(instance).name(); }
        else if constexpr   (std::is_same_v<T, events::display>) { return std::get<T>(instance).name(); }
        else if constexpr   (std::is_same_v<T, events::keyboard>) { return std::get<T>(instance).name(); }
        else if constexpr   (std::is_same_v<T, events::mouse>) { return std::get<T>(instance).name(); }
        else if constexpr   (std::is_same_v<T, events::joystick>) { return std::get<T>(instance).name(); }
        else if constexpr   (std::is_same_v<T, events::touch>) { return std::get<T>(instance).name(); }
        else if constexpr   (std::is_same_v<T, events::sensor>) { return std::get<T>(instance).name(); }
       else
           static_assert(always_false_v<T>, "non-exhaustive visitor!");
   }, instance);

    return n;
}

std::string event::to_string(to_str_mode mode) const
{
    std::string n = std::visit([this, mode](auto&& arg)
   {
       using T = std::decay_t<decltype(arg)>;
       if constexpr        (std::is_same_v<T, events::none>) { return std::get<T>(instance).to_string(mode); }
       else if constexpr   (std::is_same_v<T, events::application>) { return std::get<T>(instance).to_string(mode); }
       else if constexpr   (std::is_same_v<T, events::window>) { return std::get<T>(instance).to_string(mode); }
       else if constexpr   (std::is_same_v<T, events::display>) { return std::get<T>(instance).to_string(mode); }
       else if constexpr   (std::is_same_v<T, events::keyboard>) { return std::get<T>(instance).to_string(mode); }
       else if constexpr   (std::is_same_v<T, events::mouse>) { return std::get<T>(instance).to_string(mode); }
       else if constexpr   (std::is_same_v<T, events::joystick>) { return std::get<T>(instance).to_string(mode); }
       else if constexpr   (std::is_same_v<T, events::touch>) { return std::get<T>(instance).to_string(mode); }
       else if constexpr   (std::is_same_v<T, events::sensor>) { return std::get<T>(instance).to_string(mode); }
       else
           static_assert(always_false_v<T>, "non-exhaustive visitor!");
   }, instance);

    return n;
}


} //END namespace cpaf::gui::events

