#pragma once
#include <vector>
#include <string>
#include <memory>
#include <chrono>
#include <filesystem>
#include <cpaf_libs/video/av_util.h>

namespace Zippy {
class ZipArchive;
}

namespace cpaf::video {


/**

@see https://github.com/troldal/KZip

@see https://en.cppreference.com/w/cpp/thread/future
@see https://en.cppreference.com/w/cpp/thread/packaged_task

*/
class subtitle_container
{
public:
    std::chrono::microseconds           subtitle_adjust_offset  {};

    struct frame_t {
        std::vector<std::string>        lines                   {};
        std::chrono::microseconds       presentation_time       {0};
        std::chrono::microseconds       presentation_time_end   {0};
        uint32_t                        sequence_number         {0};
        std::string                     to_string               () const;
    };

    using vec_t = std::vector<frame_t>;
    using iterator = vec_t::iterator;
    using const_iterator = vec_t::const_iterator;

    static std::unique_ptr<subtitle_container>
                                        create_from_path        (const std::string& resource_path,
                                                                 std::chrono::milliseconds timeout);
    static subtitle_text_format_t       detect_format           (std::string_view data_string_view);



    subtitle_container() = default;
    void                                parse_file_data         (std::string_view data_string_view);
    void                                parse_srt_file_data     (std::string_view data_string_view);

    // ----------------------
    // --- Info functions ---
    // ----------------------
    void                                language_code_set		(std::string_view lc) { language_code_ = lc; }
    const std::string&                  language_code			() const { return language_code_; }
    size_t                              size                    () const { return subtitles_.size(); }
    bool                                empty                   () const { return size() == 0u; }
    bool                                is_valid                () const;

    // ----------------------
    // --- Data functions ---
    // ----------------------
    const std::vector<frame_t>&         subtitles               () const { return subtitles_; }
    const_iterator                      find_last_before        (std::chrono::microseconds ts) const;
    const_iterator                      find_first_after        (std::chrono::microseconds ts) const;

    iterator                            begin                   ()          { return subtitles_.begin();}
    const_iterator                      begin                   () const    { return subtitles_.begin();}
    iterator                            end                     ()          { return subtitles_.end();  }
    const_iterator                      end                     () const    { return subtitles_.end();  }

private:
    enum class state_t {new_frame, seq_num, ps_times, text};
    // ------------------------
    // --- PRIVATE: Helpers ---
    // ------------------------
    void                                parse_line              (std::string_view line);
    bool                                parse_sequence_number   (std::string_view line);
    bool                                parse_presentation_times(std::string_view line);
    bool                                parse_subtitle_line     (std::string_view line);
    void                                push_current_frame      ();

    std::vector<frame_t>                subtitles_;
    std::string                         language_code_;
    frame_t                             cur_parse_frame_;
    state_t                             parse_state_                = state_t::new_frame;
};

} // namespace cpaf::video
