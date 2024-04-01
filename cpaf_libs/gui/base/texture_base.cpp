#include "texture_base.h"


namespace cpaf::gui {


texture_base::texture_base()
{

}

texture_base::texture_base(std::shared_ptr<system_render> render)
    : render_(std::move(render))
{

}

void texture_base::set_render(std::shared_ptr<system_render> render)
{
    render_ = std::move(render);
}

uint32_t texture_base::pixel_data_raw_size_bytes() const
{
    const auto sz = size();
    return static_cast<uint32_t>(sz.width()*sz.height());
}

// texture_base::texture_base(const size_2d& /*size*/, std::string_view /*title*/)
// {

// }



// -------------------------------------------------------
// --- PROTECTED: Helper functions for derived classes ---
// -------------------------------------------------------




} // END namespace cpaf::gui

