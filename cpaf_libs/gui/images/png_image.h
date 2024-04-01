#pragma once

#include <filesystem>
#include <png.h>
#include <cpaf_libs/gui/gui_types.h>

namespace cpaf::gui {

std::string         png_color_type_to_string  (uint8_t png_color_type);

class png_image
{
public:
//    enum color_type_t { };
    png_image();
    explicit png_image(std::filesystem::path local_path);
    ~png_image();

    bool                        open_local          (std::filesystem::path local_path);
    bool                        read_to_memory      ();

    int32_t                     width               () const;
    int32_t                     height              () const;
    size_2d                     size                () const                            { return { width(), height()}; }
    uint8_t                     png_color_type      () const;
    std::string                 png_color_type_str  () const                            { return png_color_type_to_string(png_color_type()); }
    uint8_t                     bit_depth           () const;
    uint32_t                    pixel               (int32_t x, int32_t y) const;
    void                        copy_pixels_out     (uint32_t* dst_buffer);

private:

    void                        alloc_row_pointers  ();
    void                        destroy             ();
    void                        close_file          ();
    void                        free_row_pointers   ();

    std::filesystem::path       local_path_         {};
    png_structp                 png_struct_ptr_     {nullptr};
    png_infop                   png_info_ptr_       {nullptr};
    png_bytep*                  row_pointers_       {nullptr};
    FILE*                       file_ptr_           {nullptr};
};

} // namespace cpaf::gui

