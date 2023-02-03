
#include "av_samples_queue.h"

#include <utility>
#include <ios>
#include <sstream>
#include <iostream>

using namespace std;
namespace cpaf::video {


// -------------------------
// --- Constructors etc. ---
// -------------------------


av_samples_queue::av_samples_queue()
{
}




av_samples_queue::~av_samples_queue()
{
}




// -----------------------
// --- Debug functions ---
// -----------------------



std::string av_samples_queue::dbg_string_short() const
{
    std::stringstream ss;
    return ss.str();
}


} //END namespace cpaf::video
