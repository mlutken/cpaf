
#include "av_frame.h"

#include <utility>
#include <ios>
#include <string>
#include <sstream>
#include <iostream>

extern "C"
{
#include <libavutil/imgutils.h>
}

#include <cpaf_libs/video/av_codec_context.h>

using namespace std;
namespace cpaf::video {


uint32_t av_frame::extract_num_counter = 0;

av_frame av_frame::create_alloc()
{
    return av_frame(create_mode::alloc, illegal_stream_index());
}

av_frame::av_frame()
{
    create_number_ = extract_num_counter++;
//    dbg_print("DEFAULT CONSTRUCT frame!");
}

av_frame::av_frame(create_mode cm, size_t stream_index)
    : stream_index_(stream_index)
{
    create_number_ = extract_num_counter++;
    if (cm == create_mode::alloc) {
        alloc_frame_struct();
//        dbg_print("ALLOC CONSTRUCT frame!");
    }
    else {
//        dbg_print("CONSTRUCT frame!");
    }
}

// -------------------------
// --- Constructors etc. ---
// -------------------------


av_frame::~av_frame()
{
    if (image_buffer_) {
        av_free(image_buffer_);
    }
    if (ff_frame_) {
//        dbg_print("DESTRUCT frame!");
        av_frame_free(&ff_frame_);
        av_freep(&ff_frame_); // TODO: Is this really needed? I think not according to av_frame_free doc it should free both the struct and the actual frame data
    }
    else {
//        dbg_print("DESTRUCT NULL frame!");
    }
}

void av_frame::swap(av_frame& src) noexcept
{
    std::swap(ff_frame_, src.ff_frame_);
    std::swap(image_buffer_, src.image_buffer_);
    std::swap(stream_index_, src.stream_index_);
    std::swap(presentation_time_, src.presentation_time_);
    std::swap(create_number_, src.create_number_);
    std::swap(pipeline_control_, src.pipeline_control_);
    std::swap(pipeline_index_, src.pipeline_index_);
}

// -----------------------------
// --- Scale image functions ---
// -----------------------------

bool av_frame::init_scaling(size_t image_size_bytes, AVPixelFormat pixel_format,
                            int32_t dst_width, int32_t dst_height, int32_t dst_image_align)
{
    image_buffer_ = (uint8_t *) av_malloc(image_size_bytes * sizeof(uint8_t));

    av_image_fill_arrays(
                ff_frame_->data,
        ff_frame_->linesize,
        image_buffer_,
        pixel_format,
        dst_width,
        dst_height,
        dst_image_align
                );
    return true;
}

/** Prepare frame as destination for a scaled frame (image)
    @note The @a codec_context needs to have called it's
    init_scaling_context() for this to work.
*/
bool av_frame::init_scaling(const av_codec_context& codec_context)
{
    if (!codec_context.scaling_context_initialized()) {
        return false;
    }
    return init_scaling(codec_context.dst_image_buffer_size(),
                        codec_context.ff_dst_pixel_format(),
                        codec_context.dst_width(),
                        codec_context.dst_height(),
                        codec_context.dst_image_align());
}

/**
Write the given frame into a .ppm file.
@note: Only works if frame has been scaled/converted to AV_PIX_FMT_RGB24 format.
*/
bool av_frame::save_as_ppm(const std::filesystem::path& file_path) const
{
    FILE *file = fopen(file_path.string().c_str(), "wb");
    if (file == nullptr)   return false;

    const auto width = video_width();
    const auto height = video_height();

    // Write header
    fprintf(file, "P6\n%d %d\n255\n", width, height);

    // Write pixel data
    for (int32_t y = 0; y < height; y++)
    {
        fwrite(ff_frame_->data[0] + y * ff_frame_->linesize[0], 1, static_cast<size_t>(width * 3), file);
    }

    fclose(file);

    return true;
}

/**
Save frame as .ppm file using auto name ( timestamp in ms) into diretory.
@note: Only works if frame has been scaled/converted to AV_PIX_FMT_RGB24 format. */
bool av_frame::save_as_ppm_auto(const std::filesystem::path& frames_dir) const
{
    using namespace std;
    string leaf = "frame_"s + std::to_string(presentation_time_ms().count()) + "_ms.ppm";
    return save_as_ppm(frames_dir / leaf);
}

// -----------------------
// --- Debug functions ---
// -----------------------

void av_frame::dump() const
{
}

std::string av_frame::dbg_string_short() const
{
    std::stringstream ss;
    ss << "[" << create_number() << "]"
//       << " stream index: '" << stream_index()
//       << std::hex
       << " , pts_stream_base: " << pts_stream_base()
       << " ;  pts: "
       << presentation_time_ms().count() << " ms"
       << ", pipeline_control: '" << to_string(pipeline_control()) << "'"
       ;
    return ss.str();

}

void av_frame::dbg_print_short() const
{
    std::cerr << dbg_string_short() << "\n";
}

void av_frame::dbg_print(const string_view msg) const
{
    std::cerr << msg
              << " this: " << this
              << " create_number: " << create_number_
              << " ff_frame_: " << ff_frame_
              << "\n";
}

// priority queue save
//std::vector<av_frame> container;
//container.reserve(20);
//std::priority_queue<av_frame, std::vector<av_frame>, std::less<av_frame>> pq(std::less<av_frame>(), std::move(container));


} //END namespace cpaf::video
