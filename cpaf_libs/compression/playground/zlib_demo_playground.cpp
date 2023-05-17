#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <iostream>
#include <vector>
#include <zlib.h>
#include <filesystem>
#include <streams/cpaf_streams.h>
#include <containers/cpaf_container_utils.h>
#include <containers/cpaf_dynamic_array.hpp>
#include <compression/cpaf_compression_functions.h>

using namespace std;
namespace fs = std::filesystem;
namespace con = cpaf::containers;
namespace com = cpaf::compression;
namespace cs = cpaf::streams;

//fs::path g_test_gz = fs::path(__FILE__).parent_path() / "test.gz";
fs::path g_test_gz = fs::path(__FILE__).parent_path() / "feed.xml.gz";
//fs::path g_test_gz = fs::path(__FILE__).parent_path() / "sportsshoes.xml.gz";

constexpr auto windowBits = 15;
constexpr auto ENABLE_ZLIB_GZIP = 32;
/* CHUNK is the size of the memory chunk used by the zlib routines. */
constexpr auto CHUNK_SIZE = 0x4000;


/* The following macro calls a zlib routine and checks the return
   value. If the return value ("status") is not OK, it prints an error
   message and exits the program. Zlib's error statuses are all less
   than zero. */

#define CALL_ZLIB(x) {                                                  \
    int status;                                                     \
    status = x;                                                     \
    if (status < 0) {                                               \
    fprintf (stderr,                                            \
    "%s:%d: %s returned a bad status of %d.\n",        \
    __FILE__, __LINE__, #x, status);                   \
    exit (EXIT_FAILURE);                                        \
    }                                                               \
    }

/* if "test" is true, print an error message and halt execution. */

#define FAIL(test,message) {                             \
    if (test) {                                      \
    inflateEnd (& strm);                         \
    fprintf (stderr, "%s:%d: " message           \
    " file '%s' failed: %s\n",          \
    __FILE__, __LINE__, file_name,      \
    strerror (errno));                  \
    exit (EXIT_FAILURE);                         \
    }                                                \
    }

int main ()
{
    auto da_in = cs::to_dynamic_array_raw<unsigned char>(g_test_gz, 0);
    auto dst_buffer = com::gz_uncompress_uchar(da_in.data(), da_in.size());
    cs::to_file(fs::path("/tmp/_zlib_demo_playground_uncompressed.xml"), dst_buffer);
    fwrite (dst_buffer.data(), sizeof (char), dst_buffer.size(), stdout);
    std::cerr << "dst_buffer.size(): " << dst_buffer.size()
              << ",  dst_buffer.capacity():  " << dst_buffer.capacity() << "\n";


    return 0;
    const char * file_name = g_test_gz.string().c_str();
    FILE * file;
    z_stream strm = {};
    unsigned char in_buffer[CHUNK_SIZE];
    unsigned char out_buffer[CHUNK_SIZE];

    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.next_in = in_buffer;
    strm.avail_in = 0;
    CALL_ZLIB (inflateInit2 (& strm, windowBits | ENABLE_ZLIB_GZIP));

    /* Open the file. */

    file = fopen (file_name, "rb");
    FAIL (! file, "open");
    while (1) {
        unsigned int bytes_read;
        int zlib_status;

        bytes_read = fread (in_buffer, sizeof (char), sizeof (in_buffer), file);
        FAIL (ferror (file), "read");
        strm.avail_in = bytes_read;
        strm.next_in = in_buffer;
        do {
            unsigned have;
            strm.avail_out = CHUNK_SIZE;
            strm.next_out = out_buffer;
            zlib_status = inflate (& strm, Z_NO_FLUSH);
            switch (zlib_status) {
            case Z_OK:
            case Z_STREAM_END:
            case Z_BUF_ERROR:
                break;

            default:
                inflateEnd (& strm);
                fprintf (stderr, "Gzip error %d in '%s'.\n",
                         zlib_status, file_name);
                return -1;
            }
            have = CHUNK_SIZE - strm.avail_out;
            fwrite (out_buffer, sizeof (unsigned char), have, stderr);
        }
        while (strm.avail_out == 0);

        if (feof (file)) {
            inflateEnd (& strm);
            break;
        }
    }
    FAIL (fclose (file), "close");
    return 0;
}

