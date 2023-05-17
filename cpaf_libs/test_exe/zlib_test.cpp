// #include <filesystem>
// #include <boost/filesystem/convenience.hpp>
// #include <zlib.h>

#include <cpaf_compression_functions.h>

using cpaf::compression::gzCompress;
using cpaf::compression::gzUnCompress;

namespace fs = std::filesystem;

/*
// using namespace std;
// using namespace cpaf;
namespace fs = std::filesystem;

/// Compress a single file to gz format using zlib. 
/// If \a dstFilePath is an existing directory the file is compressed to here with 
/// '.gz' appended to the filename.
bool gzCompress( 
	const std::filesystem::path& srcFilePath, 	///< [in] Path to source file
	const std::filesystem::path& dstFilePath  	///< [in] Path to destination file. Can be a directory name
) 
{
	namespace fs = std::filesystem;
	fs::path dstFile(dstFilePath);
	
	if ( fs::is_directory(dstFile) ) {
		dstFile /= srcFilePath.leaf() + ".gz";
	}
	
	unsigned iBlockSize = 8192;
	char buffer[iBlockSize];
	FILE*  fp 	= fopen		( srcFilePath.string().c_str(), "r"		);
	gzFile zp 	= gzopen	( dstFile.string().c_str(), 	"w9" 	);
	while (!feof(fp)) {
		unsigned iBytesRead = fread ( buffer,1, iBlockSize, fp );
		gzwrite  ( zp  , buffer, iBytesRead );		
	}
	fclose(fp);
	gzclose(zp);
}


/// Uncompress a single file from gz format using zlib. 
/// If \a dstFilePath is an existing directory the file is uncompressed to here with 
/// '.gz' removed from the filename.
bool gzUnCompress( 
	const std::filesystem::path& srcFilePath, 	///< [in] Path to source file
	const std::filesystem::path& dstFilePath  	///< [in] Path to destination file. Can be a directory name
) 
{
	namespace fs = std::filesystem;
	fs::path dstFile(dstFilePath);
	
	if ( fs::is_directory(dstFile) ) {
		dstFile /= srcFilePath.leaf();
		dstFile.replace_extension("");
	}
	
	unsigned iBlockSize = 8192;
	char buffer[iBlockSize];
	gzFile zp 	= gzopen	( srcFilePath.string().c_str(), "r" );
	FILE*  fp 	= fopen		( dstFile.string().c_str(), 	"w"	);
	while (!gzeof(zp)) {
// 								gzFile file, voidp buf, unsigned len
		unsigned iBytesRead = gzread ( zp, buffer, iBlockSize );
		fwrite  ( buffer, 1, iBytesRead, fp );		
	}
	fclose(fp);
	gzclose(zp);
}

*/

#include <iostream>

int zlib_test(int argc, char* argv[])
{
	using namespace std;
	cout << "zlib_test" << endl;
 	fs::path srcFile("GinzaSe.xml");
 	fs::path dstFile("GinzaSe.xml.gz");
//  	fs::path dstFile("/home/ml");

	gzCompress( srcFile, dstFile );
	gzUnCompress( dstFile, fs::path("GinzaUncompressed.xml") );
	return 0;
}
