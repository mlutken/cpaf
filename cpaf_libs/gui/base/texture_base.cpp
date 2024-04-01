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

// texture_base::texture_base(const size_2d& /*size*/, std::string_view /*title*/)
// {

// }



// -------------------------------------------------------
// --- PROTECTED: Helper functions for derived classes ---
// -------------------------------------------------------




} // END namespace cpaf::gui

