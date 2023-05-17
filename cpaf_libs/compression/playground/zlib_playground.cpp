#include <iostream>
#include <vector>
#include <filesystem>
#include <streams/cpaf_streams.h>
#include <containers/cpaf_dynamic_array.hpp>
#include <compression/cpaf_compression_functions.h>

using namespace std;
namespace fs = std::filesystem;
namespace con = cpaf::containers;
namespace com = cpaf::compression;
namespace cs = cpaf::streams;

fs::path g_in_file_txt      = fs::path(__FILE__).parent_path() / "playground.txt";
fs::path g_in_file_txt_zlib = fs::path(__FILE__).parent_path() / "playground.txt.zlib";
fs::path g_in_file_txt_zlib2= fs::path(__FILE__).parent_path() / "playground.txt.zlib2";
fs::path g_out_file_txt     = fs::path("/tmp/zlib_out_playground.txt");
fs::path g_out_file_txt_zlib= fs::path("/tmp/zlib_out_playground.txt.zlib");
fs::path g_out_file_txt_gz  = fs::path("/tmp/zlib_out_playground.txt.gz");

int main()
{
    cerr << "--- zlib playground ---\n";
    auto da_txt = cs::to_dynamic_array<unsigned char>(g_in_file_txt, 0);
    cs::to_file(g_out_file_txt, da_txt);
    cs::to_file(g_out_file_txt_gz, da_txt);


    auto da_txt_zlib = cs::to_dynamic_array<unsigned char>(g_in_file_txt_zlib2, 0);
    con::dynamic_uchar_array da = com::gz_uncompress(da_txt_zlib.data(), da_txt_zlib.size());

    cs::to_file(g_out_file_txt_zlib, da_txt_zlib);

    return 0;
}

