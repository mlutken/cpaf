

#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <set>
#include <algorithm>

#include <boost/program_options.hpp>
#include <boost/lambda/lambda.hpp>

#include <cpaf_file_directory_functions.h>
#include <cpaf_special_dirs.h>
#include <cpaf_string_utils.hpp>


using namespace std;
using namespace boost::lambda;
using namespace cpaf::filesystem;
namespace po = boost::program_options;
namespace fs = std::filesystem;


typedef std::vector<std::string> 	StringVecT;
typedef std::set<std::string> 		StringSetT;

// --- Global variables (well this is just a simple command line program so ...) ---
// NOTE: This program could also be optimized to some extent - but for now we keep it like this!
int				recurDepth;
StringVecT   	matchFiles,   matchFilesRe;
StringVecT   	matchDirs,    matchDirsRe;
StringVecT   	matchPaths,   matchPathsRe;
StringVecT   	excludeFiles, excludeFilesRe;
StringVecT   	excludeDirs,  excludeDirsRe;
StringVecT   	excludePaths, excludePathsRe;
StringVecT   	compareDirsVec;



static filepaths_t filterAll ( const filepaths_t& filePaths )
{
	filepaths_t filteredPaths;
	filepaths_t::const_iterator itc = filePaths.begin();
	for ( ; itc != filePaths.end(); ++itc ){
		
		string fullPathName = itc->string();
        string leafFileName = fs::path( itc->filename() ).string();
        string leafDirName 	= fs::path( itc->branch_path().filename()).string();
		////cout << "["<< *itc << "]  " << leafDirName << " / " << leafFileName << endl;
		
		StringVecT::const_iterator itCheck; 
		
		// *********************		
		// *** Exclude files ***
		// *********************		
		bool bExcludePath = false;
		// --- exclude ---
		for ( itCheck = excludeFiles.begin(); itCheck != excludeFiles.end(); ++itCheck ) {
			if ( !itCheck->empty() && cpaf::wildcmp( leafFileName, *itCheck ) ) { bExcludePath = true; break; }
		}
		for ( itCheck = excludeDirs.begin(); itCheck != excludeDirs.end(); ++itCheck ) {
			if ( !itCheck->empty() && cpaf::wildcmp( leafDirName, *itCheck ) ) { bExcludePath = true; break; }
		}
		for ( itCheck = excludePaths.begin(); itCheck != excludePaths.end(); ++itCheck ) {
			if ( !itCheck->empty() && cpaf::wildcmp( fullPathName, *itCheck ) ) { bExcludePath = true; break; }
		}
		// --- excludeRe ---
		for ( itCheck = excludeFilesRe.begin(); itCheck != excludeFilesRe.end(); ++itCheck ) {
			if ( !itCheck->empty() && boost::regex_match( leafFileName, boost::regex ( *itCheck ) ) ) {	bExcludePath = true; break; }
		}
		for ( itCheck = excludeDirsRe.begin(); itCheck != excludeDirsRe.end(); ++itCheck ) {
			if ( !itCheck->empty() && boost::regex_match( leafDirName, boost::regex ( *itCheck ) ) ) {	bExcludePath = true; break; }
		}
		for ( itCheck = excludePathsRe.begin(); itCheck != excludePathsRe.end(); ++itCheck ) {
			if ( !itCheck->empty() && boost::regex_match( fullPathName, boost::regex ( *itCheck ) ) ) {	bExcludePath = true; break; }
		}
		
		if ( bExcludePath )	continue;
		
		// *******************************		
		// *** Match/Add/Include files ***
		// *******************************		
		bool bAddPath = false;
		// If all match vectors are empty (ie. no match parameters entered on command line we force match the file)
		if ( matchFiles.empty()   && matchDirs.empty()   && matchPaths.empty()   && 
			 matchFilesRe.empty() && matchDirsRe.empty() && matchPathsRe.empty()    ) bAddPath = true;
		
		// --- match ---
		for ( itCheck = matchFiles.begin(); itCheck != matchFiles.end(); ++itCheck ) {
			if ( itCheck->empty() || cpaf::wildcmp( leafFileName, *itCheck ) ) {	bAddPath = true; break; }
		}
		for ( itCheck = matchDirs.begin(); itCheck != matchDirs.end(); ++itCheck ) {
			if ( itCheck->empty() || cpaf::wildcmp( leafDirName, *itCheck ) ) {	bAddPath = true; break; }
		}
		for ( itCheck = matchPaths.begin(); itCheck != matchPaths.end(); ++itCheck ) {
			if ( itCheck->empty() || cpaf::wildcmp( fullPathName, *itCheck ) ) {	bAddPath = true; break; }
		}
		// --- matchRe ---
		for ( itCheck = matchFilesRe.begin(); itCheck != matchFilesRe.end(); ++itCheck ) {
			if ( itCheck->empty() || boost::regex_match( leafFileName, boost::regex ( *itCheck ) ) ) {	bAddPath = true; break; }
		}
		for ( itCheck = matchDirsRe.begin(); itCheck != matchDirsRe.end(); ++itCheck ) {
			if ( itCheck->empty() || boost::regex_match( leafDirName, boost::regex ( *itCheck ) ) ) {	bAddPath = true; break; }
		}
		for ( itCheck = matchPathsRe.begin(); itCheck != matchPathsRe.end(); ++itCheck ) {
			if ( itCheck->empty() || boost::regex_match( fullPathName, boost::regex ( *itCheck ) ) ) {	bAddPath = true; break; }
		}
		
		if ( bAddPath) filteredPaths.push_back( *itc );
	}
	return filteredPaths;
}



static void compareDirs()
{
	fs::path  		comparePathFrom;
	fs::path  		comparePathTo;
	filepaths_t  	compareFromFiles;
	filepaths_t  	compareToFiles;
	StringSetT  	compareFromFilesSet;	// For easy and fast checking whether the file exists
	StringSetT  	compareToFilesSet;		// For easy and fast checking whether the file exists
	filepaths_t  	filesAdded;
	filepaths_t  	filesDeleted;

	if ( compareDirsVec.size() != 2 ) { cout << "Error: You must provide exactly two input directories for comparing!\n"; return; }

	cout << "Exclude patterns: ";
	for_each ( excludeFilesRe.begin(), excludeFilesRe.end(), cout << _1 << "   " );
	cout<<endl;

	cout << "Directories to compare: ";
	for_each ( compareDirsVec.begin(), compareDirsVec.end(), cout << _1 << "  " );
	cout<<endl;
	////	re = boost::regex(".*\\.c");



	comparePathFrom = fs::path ( compareDirsVec[0]) ;
	comparePathTo	= fs::path ( compareDirsVec[1]) ;
	cout << "comparePathFrom : " << comparePathFrom.string() << endl;
	cout << "comparePathTo   : " << comparePathTo.string() << endl;
	
	compareFromFiles 	= files_list_recursive ( comparePathFrom, true, boost::regex(), recurDepth );
	compareToFiles 		= files_list_recursive ( comparePathTo, true, boost::regex(), recurDepth );
	
	compareFromFiles	= filterAll ( compareFromFiles );
	compareToFiles		= filterAll ( compareToFiles );
	
	
	filepaths_t::iterator it = compareFromFiles.begin();
	////cout << "Files in from directory:" << endl;
	for ( ; it != compareFromFiles.end(); ++it ){
		*it = remove_from_start ( *it, comparePathFrom );
		compareFromFilesSet.insert(it->string());
		////cout << "From file: " << (*it).string() << endl;
	}
	cout << endl;
		
	it = compareToFiles.begin();
	////cout << "Files in to directory:" << endl;
	for ( ; it != compareToFiles.end(); ++it ){
		*it = remove_from_start ( *it, comparePathTo );
		compareToFilesSet.insert(it->string());
		////cout << "To file: " << (*it).string() << endl;
	}
	cout << endl;
		
	// --- Find deleted files (in 'from' but NOT in 'to') ---	
	filepaths_t::const_iterator itc = compareFromFiles.begin();
	for ( ; itc != compareFromFiles.end(); ++itc ){
		if ( compareToFilesSet.find(itc->string()) == compareToFilesSet.end() )  {
			filesDeleted.push_back( *itc );
		}
	}
		
	// --- Find added files (in 'to' but NOT in 'from') ---	
	itc = compareToFiles.begin();
	for ( ; itc != compareToFiles.end(); ++itc ){
		if ( compareFromFilesSet.find(itc->string()) == compareFromFilesSet.end() )  {
			filesAdded.push_back( *itc );
		}
	}
		
	cout << "Files deleted (in 'from' but NOT in 'to'): " << filesDeleted.size() << endl;
	itc = filesDeleted.begin();
	for ( ; itc != filesDeleted.end(); ++itc ){
		cout << (*itc).string() << endl;
	}
	cout << endl;
		
	cout << "Files added (in 'to' but NOT in 'from'): " << filesAdded.size() << endl;
	itc = filesAdded.begin();
	for ( ; itc != filesAdded.end(); ++itc ){
		cout << (*itc).string() << endl;
	}
	cout << endl;
}




int main ( int ac, char* av[] )
{
	try
	{
		po::options_description generic ( "General options" );
		generic.add_options()
		( "help", "Print this help." )
		("depth", po::value<int>(&recurDepth)->default_value(-1), "Recursion depth. Default (-1) means infinite.")
		;
		po::options_description wildcard ( "Wildcard/Globbing matching/excluding options" );
		wildcard.add_options()
		( "files-match,f", po::value<StringVecT> ( &matchFiles ), "Match leaf file name ( wildcards *,? useable)" )
		( "dirs-match,d", po::value<StringVecT> ( &matchDirs), "Match leaf directory name ( wildcards *,? useable)" )
		( "paths-match,p", po::value<StringVecT> ( &matchPaths), "Match full path name ( wildcards *,? useable)" )
		( "files-exclude,F", po::value<StringVecT> ( &excludeFiles ), "Exclude leaf file name ( wildcards *,? useable)" )
		( "dirs-exclude,D", po::value<StringVecT> ( &excludeDirs), "Exclude leaf directory name ( wildcards *,? useable)" )
		( "paths-exclude,P", po::value<StringVecT> ( &excludePaths), "Exclude full path name ( wildcards *,? useable)" )
		;
		
		po::options_description regex ( "Regular expression matching/excluding options" );
		regex.add_options()
		( "files-match-re", po::value<StringVecT> ( &matchFilesRe ), "Match leaf file name (regular expression)" )
		( "dirs-match-re", po::value<StringVecT> ( &matchDirsRe), "Match leaf directory name (regular expression)" )
		( "paths-match-re", po::value<StringVecT> ( &matchPathsRe), "Match full path name (regular expression)" )
		( "files-exclude-re", po::value<StringVecT> ( &excludeFilesRe ), "Exclude leaf file name (regular expression)" )
		( "dirs-exclude-re", po::value<StringVecT> ( &excludeDirsRe), "Exclude leaf directory name (regular expression)" )
		( "paths-exclude-re", po::value<StringVecT> ( &excludePathsRe), "Exclude full path name (regular expression)" )
		;
		
		po::options_description hidden("Hidden options");
		hidden.add_options()
		( "directory", po::value<StringVecT> ( &compareDirsVec ), "Directory to compare" )
		;        



        po::options_description cmdline_options;
        cmdline_options.add(generic).add(wildcard).add(regex).add(hidden);

        po::options_description visible("Compare two directories for new or deleted files.\nSYNTAX:\n  dircomp [options] dirFrom dirTo\n\nEXAMPLES:\n  dircomp --files-match *.c dirFromName dirToName\n  dircomp --files-exclude *.h dirFromName dirToName\n  dircomp --files-match-re .*\\\\.c dirFromName dirToName\n\nOPTIONS");
        visible.add(generic).add(wildcard).add(regex);
        
        po::positional_options_description p;
        p.add("directory", -1);
        
        po::variables_map vm;
        store(po::command_line_parser(ac, av).
              options(cmdline_options).positional(p).run(), vm);
		po::notify ( vm );


		if ( vm.count ( "help" ) )
		{
			cout << visible << "\n";
			cout << "sizeof(int*): " << sizeof(int*) << "\n";
			return 1;
		}
	}
	catch ( exception& e )
	{
		cerr << "error: " << e.what() << "\n";
		return 1;
	}
	catch ( ... )
	{
		cerr << "Exception of unknown type!\n";
	}

	compareDirs();

	return 0;
}











