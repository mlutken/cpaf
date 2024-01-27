#pragma once
#include <vector>
#include <string>
#include <memory>
#include <filesystem>


namespace Zippy {
class ZipArchive;
}

namespace cpaf::video {


/**

@see https://github.com/troldal/KZip


*/
class subtitle_container
{
public:

    struct frame_t {
        std::vector<std::string>        lines                   {};
        std::chrono::microseconds       presentation_time       {0};
        std::chrono::microseconds       presentation_time_end   {0};
        uint32_t                        sequence_number         {0};
    };


    subtitle_container() = default;

    void                            parse_srt_file_data     (std::string_view data_string_view);


    // ----------------------
    // --- Info functions ---
    // ----------------------
    void                            language_code_set		(std::string_view lc) { language_code_ = lc; }
    const std::string&              language_code			() const { return language_code_; }

    // ----------------------
    // --- Data functions ---
    // ----------------------
    const std::vector<frame_t>&     subtitles               () const { return subtitles_; }

    // --- ---

private:
    enum class state_t {new_frame, seq_num, ps_times, text, frame_done};
    // ------------------------
    // --- PRIVATE: Helpers ---
    // ------------------------
    void                            parse_line              (std::string_view line);
    bool                            parse_sequence_number   (std::string_view line);
    bool                            parse_presentation_times(std::string_view line);
    bool                            parse_subtitle_line     (std::string_view line);
    void                            push_current_frame      ();

    std::vector<frame_t>            subtitles_;
    std::string                     language_code_;
    frame_t                         cur_parse_frame_;
    state_t                         parse_state_ = state_t::new_frame;
};

} // namespace cpaf::video
