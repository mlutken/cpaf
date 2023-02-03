#include <iostream>
#include <png.h>
#include <test_lib.h>

#define CURL_STATICLIB
#include <stdio.h>
#include <curl/curl.h>
//#include <curl/types.h>
#include <curl/easy.h>
#include <string>






size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int main(void) {
    CURL *curl;
    FILE *fp;
    CURLcode res;
    const char* url = "http://examples.netscavator.com/ExamplesText.txt";
    char outfilename[FILENAME_MAX] = "/home/ml/temp/aaa.txt";
    curl = curl_easy_init();
    if (curl) {
        fp = fopen(outfilename,"wb");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
    }
    return 0;
}
















// ---------------------------------
// --- OLD Curl POST upload test ---
// ---------------------------------
//// #include <boost/filesystem/path.hpp>
//// #include <curl/curl.h>
//#include <cpaf_http_convenience.h>

//using cpaf::net::curlPostUploadFileSimple;

//using namespace std;
//// using namespace cpaf;
//namespace fs = boost::filesystem;


//#include <iostream>

//int curl_test(int argc, char* argv[])
//{
//	cout << "curl_test" << endl;
//// 	std::string sURL = "http://filesupload.airplay.localhost/upload.php";
//	std::string sURL = "http://filesupload.airplaymusic.dk/upload.php";
// 	fs::path file("MegastoreSe.xml");

//	curlPostUploadFileSimple( sURL, "uploadedfile", file );
//	return 0;
//}
