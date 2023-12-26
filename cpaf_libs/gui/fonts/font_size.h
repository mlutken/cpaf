#pragma once

namespace cpaf::gui {

class font_size
{
public:
    enum class mode_t {points, relative};
    font_size() = default;

private:
    float   size_{11};
    mode_t  mode_ = mode_t::points;
};

} // namespace cpaf::gui
