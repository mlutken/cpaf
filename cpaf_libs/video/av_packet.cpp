
#include "av_packet.h"

#include <utility>
#include <ios>
#include <sstream>
#include <iostream>

using namespace std;
namespace cpaf::video {


uint32_t av_packet::extract_num_counter = 0;

av_packet av_packet::alloc()
{
    return av_packet(create_mode::alloc);
}

// -------------------------
// --- Constructors etc. ---
// -------------------------

av_packet::av_packet()
{
    create_number_ = extract_num_counter++;
}

av_packet::av_packet(create_mode cm)
{
    create_number_ = extract_num_counter++;
    if (cm == create_mode::alloc) {
        alloc_packet_struct();
    }
}

av_packet::av_packet(av_packet&& moving) noexcept
{
    moving.swap(*this);
}


av_packet& av_packet::operator=(av_packet&& moving) noexcept
{
    create_number_ = moving.create_number_;
    media_type_ = moving.media_type_;
    moving.swap(*this);
    return *this;
}



av_packet::~av_packet()
{
    if (ff_packet_) {
        av_packet_free(&ff_packet_);
        av_freep(&ff_packet_); // TODO: Is this really needed? I think not according to av_packet_free doc it should free both the struct and the actual packet data
    }
}

void av_packet::swap(av_packet& src) noexcept
{
    std::swap(ff_packet_, src.ff_packet_);
    std::swap(create_number_, src.create_number_);
    std::swap(media_type_, src.media_type_);
    std::swap(presentation_time_, src.presentation_time_);
}

// -----------------------
// --- Debug functions ---
// -----------------------

void av_packet::dump() const
{
}


std::string av_packet::dbg_string_short() const
{
    std::stringstream ss;
    ss << "[" << create_number() << "]"
       << " media_type: '" << to_string(media_type_get())
//       << std::hex
       << ", pts: " << presentation_time_ms().count() << " ms"
       << ", pts_stream_base: " << pts_stream_base()
       ;
    return ss.str();
}

void av_packet::dbg_print_short() const
{
    std::cerr << dbg_string_short() << "\n";
}


void av_packet::dbg_print(const string_view msg) const
{
    std::cerr << msg
              << " this: " << this
              << " create_number: " << create_number_
              << " ff_packet_: " << ff_packet_
              << "\n";
}

/** Explicitly destroy (delete) the packet. Used only internally .
    for example in av_format_context::read_frame() */
void av_packet::destroy()
{
    if (ff_packet_) {
        av_packet_free(&ff_packet_);
        av_freep(&ff_packet_); // TODO: Is this really needed? I think not according to av_packet_free doc it should free both the struct and the actual packet data
        ff_packet_ = nullptr;
    }

}



// priority queue save
//std::vector<av_packet> container;
//container.reserve(20);
//std::priority_queue<av_packet, std::vector<av_packet>, std::less<av_packet>> pq(std::less<av_packet>(), std::move(container));


} //END namespace cpaf::video
