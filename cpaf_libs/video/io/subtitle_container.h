#pragma once
#include <vector>
#include <string>
#include <memory>
#include <filesystem>

#include <cpaf_libs/video/av_util.h>
#include <cpaf_libs/video/io/subtitle_text_file_data.h>

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

    subtitle_container() = default;


    // ----------------------
    // --- Info functions ---
    // ----------------------

    // ----------------------
    // --- Data functions ---
    // ----------------------

    // --- ---

private:
    // ------------------------
    // --- PRIVATE: Helpers ---
    // ------------------------

};

} // namespace cpaf::video
