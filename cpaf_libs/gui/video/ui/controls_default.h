#pragma once
#include <cpaf_libs/gui/video/ui/controls.h>

namespace cpaf::gui::video {

class controls_default : public controls
{
public:
    using controls::controls;	// "Import" constructors to scope

private:
    void    do_render() override;

};

} // namespace cpaf::gui::video
