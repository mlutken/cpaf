#include <iostream>
#include <boost/assign/list_of.hpp> 
#include <boost/format.hpp>
#include <boost/filesystem/convenience.hpp>
#include <filesystem/special_dirs.h>


#include "../Url.h"

using namespace crawl::Url;	// Using clauses like this is ok in test files, else better to put in function perhaps
using namespace std;
using boost::format;

#if 0
static const StringVecT				TestUrls = boost::assign::list_of 
("http://www.tv2.dk/nyheder/index.html")
("https://www.tv2.dk")
("www.tv2.dk")
("https://tv2.dk/nyheder/index.html?login=ml&password=mypasswd")
("http://www.bbc.co.uk/")
("/nyheder/index.html?login=ml&password=mypasswd")
("../nyheder/index.html?login=ml&password=mypasswd")
("./nyheder/index.html?login=ml&password=mypasswd")
("https://www.tv2.dk/nyheder//index.html")
("https://www.tv2.dk/nyheder/index.html?login=ml")
("https://www.tv2.dk/nyheder/index.html?login=ml&password=mypasswd")
("https://www.tv2.dk/nyheder/?login=ml&password=mypasswd")
("https://www.tv2.dk/?login=ml&password=mypasswd")
("http://www.nyheder.tv2.dk")
("http://www.nyheder.tv2.dk/")
("http://www.nyheder.tv2.dk/.")
("http://www.nyheder.tv2.dk/.")
("www.tv2.dk/nyheder")
("www.tv2.dk/nyheder/")
("www.tv2.dk/nyheder/.")
("www.tv2.dk/nyheder/..")
("https://www.tv2.dk///nyheder///index.html")
;

#else 
static const StringVecT				TestUrls = boost::assign::list_of 
("https://www.tv2.dk/nyheder/index.html?login=ml")
("https://www.tv2.dk/nyheder/index.html?login=ml&password=mypasswd")
;
#endif
void test_Url()
{
	cout<<"Testing Url class"<<endl;
	namespace fs = boost::filesystem;
//	fs::path p("/home/ml/") / fs::path("test_Url");	
//	fs::path p("C:\\Documents and Settings\\mlu") ;	
//	p /= fs::path("test_Url");
	fs::path p = cpaf::filesystem::special_dirs::home() / fs::path("test_Url");

	for ( StringVecT::const_iterator it = TestUrls.begin(); it != TestUrls.end(); ++it ) {
		cout<<"Test             : "<<*it<<endl;		
		Url Url(*it);
		cout<<"string           : "<<Url.string()<<endl; 
		cout<<"Protocol         : "<<Url.protocol()<<endl; 
		cout<<"Domain           : "<<Url.domainAsString()<<endl; 
		cout<<"Top Domain       : "<<Url.topDomain()<<endl; 
		cout<<"2nd Level Domain : "<<Url.secondLevelDomain()<<endl; 
		cout<<"Sub Domains      : "<<Url.subDomains()<<endl; 
		cout<<"Path             : "<<Url.path()<<endl; 
		cout<<"Save path        : "<<Url.savePath()<<endl; 
		cout<<"Parameters       : "<<Url.parametersAsString()<<endl; 
		fs::path p1 = p /  Url.savePath();
		fs::create_directories( p1 );
		cout<<endl;
	}
}
