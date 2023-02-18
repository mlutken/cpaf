
#include "play_stream.h"

#include <utility>

using namespace std;
namespace cpaf::video {

play_stream::play_stream(const std::string& resource_path)
    : format_context_(resource_path)
{

}

bool play_stream::open(const std::string& resource_path)
{
    return format_context_.open(resource_path);
}

// -------------------------
// --- Constructors etc. ---
// -------------------------

// -----------------------
// --- Debug functions ---
// -----------------------


} //END namespace cpaf::video
