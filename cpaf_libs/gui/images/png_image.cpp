#include "png_image.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

namespace cpaf::gui {


void png_image::read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    memory_file_t *mem = (memory_file_t *)png_get_io_ptr(png_ptr);
    if (mem->offset + length <= mem->size) {
        memcpy(data, mem->data + mem->offset, length);
        mem->offset += length;
    } else {
        png_error(png_ptr, "Read error: reached end of memory");
    }
}

std::string png_color_type_to_string(uint8_t png_color_type)
{
    switch (png_color_type) {
    case PNG_COLOR_TYPE_GRAY        : return "gray"; break;
    case PNG_COLOR_TYPE_PALETTE     : return "palette"; break;
    case PNG_COLOR_TYPE_RGB         : return "rgb"; break;
    case PNG_COLOR_TYPE_RGB_ALPHA   : return "rgba"; break;
    case PNG_COLOR_TYPE_GRAY_ALPHA  : return "gray_alpha"; break;
    default:
        break;
    }
    return "";
}


png_image::png_image()
{

}

png_image::png_image(std::filesystem::path local_path, open_mode_t open_mode)
{
    open_local(std::move(local_path), open_mode);
}

png_image::~png_image()
{
    destroy();
}

bool png_image::open_from_memory(const unsigned char* const png_data, size_t data_size, open_mode_t open_mode)
{
    destroy();
    png_struct_ptr_ = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_struct_ptr_) {
        destroy();
        std::cerr << "LOG_ERR png_create_read_struct failed\n";
        return false;
    }

    png_info_ptr_ = png_create_info_struct(png_struct_ptr_);
    if (!png_info_ptr_) {
        destroy();
        std::cerr << "LOG_ERR png_create_info_struct failed\n";
        return false;
    }


    if (setjmp(png_jmpbuf(png_struct_ptr_))) {
        png_destroy_read_struct(&png_struct_ptr_, &png_info_ptr_, NULL);
        destroy();
        std::cerr << "LOG_ERR Error during init_io\n";
        return false;
    }

    memory_file_ = {png_data, data_size, 0};

    png_set_read_fn(png_struct_ptr_, &memory_file_, png_image::read_data);
    png_read_info(png_struct_ptr_, png_info_ptr_);

    if (open_mode == open_mode_t::read_all) {
        return read_to_memory();
    }

    return true;
}

bool png_image::open_from_memory(const std::span<const unsigned char> data, open_mode_t open_mode)
{
    return open_from_memory(data.data(), data.size_bytes(), open_mode);
}

bool png_image::open_local(std::filesystem::path local_path, open_mode_t open_mode)
{
    destroy();
    local_path_ = (std::move(local_path));
    file_ptr_ = fopen(local_path_.string().c_str(), "rb");
    if (!file_ptr_) {
        std::cerr << "LOG_ERR Couldn't open '" << local_path_ << "' for reading\n";
        return false;
    }

    png_struct_ptr_ = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_struct_ptr_) {
        destroy();
        std::cerr << "LOG_ERR png_create_read_struct failed\n";
        return false;
    }

    png_info_ptr_ = png_create_info_struct(png_struct_ptr_);
    if (!png_info_ptr_) {
        destroy();
        std::cerr << "LOG_ERR png_create_info_struct failed\n";
        return false;
    }

    if (setjmp(png_jmpbuf(png_struct_ptr_))) {
        png_destroy_read_struct(&png_struct_ptr_, &png_info_ptr_, NULL);
        destroy();
        std::cerr << "LOG_ERR Error during init_io\n";
        return false;
    }

    png_init_io(png_struct_ptr_, file_ptr_);
    png_read_info(png_struct_ptr_, png_info_ptr_);

    if (open_mode == open_mode_t::read_all) {
        return read_to_memory();
    }

    return true;
}

bool png_image::read_to_memory()
{
    png_byte img_color_type = png_color_type();
    png_byte img_bit_depth = bit_depth();

    if (img_bit_depth == 16)
        png_set_strip_16(png_struct_ptr_);

    if (img_color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png_struct_ptr_);

    if (img_color_type == PNG_COLOR_TYPE_GRAY && img_bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png_struct_ptr_);

    if (png_get_valid(png_struct_ptr_, png_info_ptr_, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png_struct_ptr_);

    if (img_color_type == PNG_COLOR_TYPE_RGB || img_color_type == PNG_COLOR_TYPE_GRAY || img_color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_filler(png_struct_ptr_, 0xFF, PNG_FILLER_AFTER);

    if (img_color_type == PNG_COLOR_TYPE_GRAY || img_color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png_struct_ptr_);

    png_read_update_info(png_struct_ptr_, png_info_ptr_);

    alloc_row_pointers();

    png_read_image(png_struct_ptr_, row_pointers_);
    close_file();
    return true;
}

int32_t png_image::width() const
{
    return static_cast<int32_t>(png_get_image_width(png_struct_ptr_, png_info_ptr_));
}

int32_t png_image::height() const
{
    return static_cast<int32_t>(png_get_image_height(png_struct_ptr_, png_info_ptr_));
}

/**
 * @brief Get png color type
 * @return One of PNG_COLOR_TYPE_GRAY, PNG_COLOR_TYPE_PALETTE, PNG_COLOR_TYPE_RGB,
 *          PNG_COLOR_TYPE_RGB_ALPHA, PNG_COLOR_TYPE_GRAY_ALPHA
 */
uint8_t png_image::png_color_type() const
{
    return png_get_color_type(png_struct_ptr_, png_info_ptr_);
}

/**
 * @brief Get png image bit depth
 * @return Bit depth.
 */
uint8_t png_image::bit_depth() const
{
    return png_get_bit_depth(png_struct_ptr_, png_info_ptr_);
}

uint32_t png_image::pixel(int32_t x, int32_t y) const
{
    png_bytep row = row_pointers_[y];
    png_bytep px = &(row[x * 4]); // RGBA format
    uint32_t* rgba_pixel_ptr = reinterpret_cast<uint32_t*>(px);
//    printf("Pixel at (%d, %d): R:%d, G:%d, B:%d, A:%d\n", x, y, px[0], px[1], px[2], px[3]);
    return *rgba_pixel_ptr;
}

void png_image::copy_pixels_out(uint32_t* dst_buffer)
{
    const auto r = rect_i32({{0,0}, size()});
    const auto x_end = r.lower_right().x();
    const auto y_end = r.lower_right().y();
    for (auto y = r.top_left().y(); y < y_end; ++y) {
        for (auto x = r.top_left().x(); x < x_end; ++x) {
            const uint32_t col = pixel(x, y);
            *dst_buffer = col;
            ++dst_buffer;
        }
    }
}

void png_image::alloc_row_pointers()
{
    free_row_pointers();
    const auto img_height = static_cast<uint32_t>(height());
    row_pointers_ = (png_bytep*)malloc(sizeof(png_bytep) * img_height);
    for (auto y = 0u; y < img_height; ++y) {
        row_pointers_[y] = (png_byte*)malloc(png_get_rowbytes(png_struct_ptr_, png_info_ptr_));
    }
}

void png_image::destroy()
{
    free_row_pointers();
    if (png_info_ptr_ && png_struct_ptr_) {
        png_destroy_read_struct(&png_struct_ptr_, &png_info_ptr_, nullptr);
    }
    if (png_struct_ptr_) {
        png_destroy_read_struct(&png_struct_ptr_, nullptr, nullptr);
    }
    close_file();
    png_info_ptr_ = nullptr;
    png_struct_ptr_ = nullptr;
}

void png_image::close_file()
{
    if (file_ptr_) {
        fclose(file_ptr_);
        file_ptr_ = nullptr;
    }
}

void png_image::free_row_pointers()
{
    if (!row_pointers_) {
        return;
    }
    const auto img_height = static_cast<uint32_t>(height());
    for (auto y = 0u; y < img_height; ++y) {
        free(row_pointers_[y]);
    }
    free(row_pointers_);
    row_pointers_ = nullptr;
}


} // namespace cpaf::gui
