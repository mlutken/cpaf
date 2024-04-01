#include "png_image.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

namespace cpaf::gui {


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


void read_png(const char* filename) {
    FILE* file_ptr_ = fopen(filename, "rb");
    if (!file_ptr_) {
        printf("Error: Couldn't open %s for reading\n", filename);
        return;
    }

    png_structp png_struct_ptr_ = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_struct_ptr_) {
        fclose(file_ptr_);
        printf("Error: png_create_read_struct failed\n");
        return;
    }

    png_infop png_info_ptr_ = png_create_info_struct(png_struct_ptr_);
    if (!png_info_ptr_) {
        png_destroy_read_struct(&png_struct_ptr_, NULL, NULL);
        fclose(file_ptr_);
        printf("Error: png_create_info_struct failed\n");
        return;
    }

    if (setjmp(png_jmpbuf(png_struct_ptr_))) {
        png_destroy_read_struct(&png_struct_ptr_, &png_info_ptr_, NULL);
        fclose(file_ptr_);
        printf("Error: Error during init_io\n");
        return;
    }

    png_init_io(png_struct_ptr_, file_ptr_);
    png_read_info(png_struct_ptr_, png_info_ptr_);

    const auto width = png_get_image_width(png_struct_ptr_, png_info_ptr_);
    const auto height = png_get_image_height(png_struct_ptr_, png_info_ptr_);
    png_byte color_type = png_get_color_type(png_struct_ptr_, png_info_ptr_);
    png_byte bit_depth = png_get_bit_depth(png_struct_ptr_, png_info_ptr_);

    if (bit_depth == 16)
        png_set_strip_16(png_struct_ptr_);

    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png_struct_ptr_);

    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png_struct_ptr_);

    if (png_get_valid(png_struct_ptr_, png_info_ptr_, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png_struct_ptr_);

    if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_filler(png_struct_ptr_, 0xFF, PNG_FILLER_AFTER);

    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png_struct_ptr_);

    png_read_update_info(png_struct_ptr_, png_info_ptr_);

    const auto h = sizeof(png_bytep) * static_cast<uint32_t>(height);
    const auto w = png_get_rowbytes(png_struct_ptr_, png_info_ptr_);

    png_bytep* row_pointers_ = (png_bytep*)malloc(h);
    for (auto y = 0u; y < height; y++) {
        row_pointers_[y] = (png_byte*)malloc(w);
    }

    png_read_image(png_struct_ptr_, row_pointers_);

    fclose(file_ptr_);

    // Extract pixel data
    for (auto y = 0u; y < height; y++) {
        png_bytep row = row_pointers_[y];
        for (auto x = 0u; x < width; x++) {
            png_bytep px = &(row[x * 4]); // RGBA format
//            printf("Pixel at (%d, %d): R:%d, G:%d, B:%d, A:%d\n", x, y, px[0], px[1], px[2], px[3]);
        }
    }

    // Clean up
    for (auto y = 0u; y < height; y++) {
        free(row_pointers_[y]);
    }
    free(row_pointers_);
}



png_image::png_image()
{

}

png_image::png_image(std::filesystem::path local_path)
{
    open_local(std::move(local_path));
}

png_image::~png_image()
{
    destroy();
}

bool png_image::open_local(std::filesystem::path local_path)
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
