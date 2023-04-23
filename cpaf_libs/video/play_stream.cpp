
#include "play_stream.h"

#include <utility>

using namespace std;
namespace cpaf::video {

// -------------------------
// --- Constructors etc. ---
// -------------------------

bool play_stream::open(const std::string& resource_path)
{
    return format_context_.open(resource_path);
}


// -----------------------
// --- Video functions ---
// -----------------------
void play_stream::render_dimensions_set(const surface_dimensions_t& dim)
{
    render_dimensions_ = dim;
}

void play_stream::render_width_set(int32_t render_width)
{
    render_dimensions_.x(render_width);
    render_dimensions_.y(0);
}

void play_stream::render_height_set(int32_t render_height)
{
    render_dimensions_.x(0);
    render_dimensions_.y(render_height);
}


// -----------------------
// --- Debug functions ---
// -----------------------


} //END namespace cpaf::video
