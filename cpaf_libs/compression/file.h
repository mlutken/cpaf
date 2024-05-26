#pragma once
 
#include <string>
#include <vector>
#include <variant>
#include <cstdio>
#include <filesystem>

#include <compression/compression_utils.h>
#include <containers/cpaf_container_utils.h>
#include <containers/cpaf_dynamic_array.hpp>
#include <containers/cpaf_string8.h>

//class gzFile;

namespace cpaf { namespace compression {

/** \unit cpaf_compression_functions.hpp
! Functions for primitive compression support. 
\author Martin Lutken 
*/

struct file_handle_t;


// ---------------------------------------------------------
// --- Generic file class (handles any compression type) ---
// ---------------------------------------------------------
/** File class that can read normal as well as compressed
    files.
    The maximum line lenght to read is set with max_readline_size and
    by default this is set to DEFAULT_MAX_LINE_LENGHT = 8096.
    It can be changed both via constructor and member function
    max_readline_size().
 */
struct file
{
    static constexpr int64_t DEFAULT_MAX_LINE_LENGHT = 8096;

    file();
    explicit file(const std::string& file_path);
    explicit file(const std::filesystem::path& file_path);
    file(const std::string& file_path, const std::string& mode);
    file(const std::filesystem::path& file_path, const std::string& mode);
    file(const std::string& file_path, const std::string& mode, std::size_t max_readline_size);
    file(const std::filesystem::path& file_path, const std::string& mode, std::size_t max_readline_size);
    ~file() noexcept;

    // ------------------------------------------------
    // --- file: Info functions, setter and getters ---
    // ------------------------------------------------
    type                file_type           () const { return file_type_; }
    void                max_readline_size   (std::size_t max_readline_size) noexcept;
    std::size_t         max_readline_size   () const noexcept { return max_readline_size_; }
    std::string_view    file_path           () const noexcept { return file_path_; }
    std::string_view    mode                () const noexcept { return mode_; }

    // -----------------------------
    // --- file: state functions ---
    // -----------------------------
    bool                open                (const std::string& mode) noexcept;
    bool                open                (const std::string& file_path, const std::string& mode) noexcept;
    bool                open                (const std::filesystem::path& file_path, const std::string& mode) noexcept;
    bool                is_open             () const noexcept {  return is_open_; }
    bool                close               () noexcept;
    bool                eof                 () const noexcept;


    // ----------------------------
    // --- file: Read functions ---
    // ----------------------------
    char*               gets                (char* buf) const noexcept;
    char*               gets                () const noexcept;
    std::string_view    readline            () const noexcept;
    std::size_t         read                (void* buffer, std::size_t size, std::size_t count);

    // -----------------------------
    // --- file: Write functions ---
    // -----------------------------
    bool                puts                (const char* buf) const noexcept;
    bool                puts                (const std::string& str) const noexcept;
    std::size_t         write               (const char* buf, std::size_t bytes_to_write) const noexcept;
    std::size_t         write               (const std::string& str, std::size_t start_pos, std::size_t bytes_to_write) const noexcept;

private:
    void                allocate_read_buffer();
    void                deallocate_read_buffer();

    std::unique_ptr<file_handle_t>  handle_;
    std::string                     file_path_;
    std::string                     mode_;
    std::size_t                     max_readline_size_ = DEFAULT_MAX_LINE_LENGHT;
    type                            file_type_;
    char*                           read_buffer_ = nullptr;
    bool                            is_open_ = false;
};




}} //END namespace cpaf::compression

