#include "system_render_base.h"

#include <fmt/format.h>>

using namespace std;
namespace cpaf::gui {

std::string system_render_base::dbg_characteristics() const
{
    auto texture_dims = maximum_texture_dimensions();
    auto s = fmt::format("--- Render characteristics ---\n");
    s += fmt::format("Texture max dimensions: [{}, {}]\n", texture_dims.width(), texture_dims.height());
    return s;
}

// -------------------------------------------------------
// --- PROTECTED: Helper functions for derived classes ---
// -------------------------------------------------------



} // END namespace cpaf::gui

