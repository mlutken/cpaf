
#include "play_stream.h"

#include <utility>

using namespace std;
namespace cpaf::video {

// -------------------------
// --- Constructors etc. ---
// -------------------------

play_stream::play_stream(get_torrents_fn get_torrents_function) :
    format_context_(get_torrents_function)
{

}

play_stream::~play_stream()
{
    format_context_.close();
}

bool play_stream::open(const std::string& resource_path, const string& subtitle_path)
{
    return format_context_.open(resource_path, subtitle_path);
}

void play_stream::close()
{
    format_context_.close();
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
