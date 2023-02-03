#include <iostream>
#include <vector>
#include <string>
#include <boost/filesystem/path.hpp>
#include <nlohmann/json.hpp>

#include <streams/cpaf_streams.h>
#include <net/cpaf_webcc_client.h>
#include <elasticsearch/es_connection.h>
#include <elasticsearch/es_utils.h>
#include <elasticsearch/records/base_record.h>
#include <elasticsearch/cpaf_elasticsearch.h>


using namespace std;
using json = nlohmann::json;
namespace fs = boost::filesystem;
namespace cs = cpaf::streams;
namespace con = cpaf::containers;
namespace cu = cpaf::unicode;
namespace es = cpaf::elasticsearch;


static const fs::path myproject_index_data_path = fs::path(__FILE__).parent_path().parent_path() / "_data/default_product_site_es_mappings.json";


int main()
{
    cerr << "myproject_index_data_path; '" << myproject_index_data_path.string() << "'\n";
    const auto create_data = cpaf::streams::to_string(myproject_index_data_path);
    cerr << create_data << "\n";
    cpaf::elasticsearch::es_connection ec;

    const std::string body2 = R"(
                             {
                                 "query" : {
                                     "term" : { "shop_id" : "jupiter.dk" }
                                 }
                             }
                             )";


    ec.pretty(true);
//    es.getb("/cycling_business/brand/_search", 3, body1).Query("from", "4");
//    es.getb("/cycling_business/brand/_search", 3, body1).Query("scroll", "1m");
    //es.getb("/cycling_localized*/staticpage/_search", 6, body1);
//    auto r1 = ec.scroll_post("/cycling_localized*/staticpage/_search", 2, "1m", es::match_all_body);
//    bool more_data = cpaf::elasticsearch::response_has_hits(r1->data());

//    auto r1 = es.session().Send(req());
//    int i = 0;
//    while (more_data) {
//        i++;
//        if (i > 4) break;
//        std::cerr << r1->data() << "\n";
//        std::cerr << "scroll_id: " << ec.scroll_id() <<  "\n" ;
//        std::cerr << "more_data: " << more_data <<  "\n" ;
//        r1 = ec.scroll_post_next();
//        more_data = cpaf::elasticsearch::response_has_hits(r1->data());
//    }
//    ec.scroll_delete();

//    ec.del("/customer/external/1", "");
//    auto& req = ec.delb("/customer/external/1", "");
//    ec.delb("/customer/external/1", "");

//    std::cerr << "index_exist 'customer' : '" << ec.index_exist("customer") <<  "'\n" ;
//    std::cerr << "index_exist 'customerX': '" << ec.index_exist("customerX") <<  "'\n" ;
//    ec.index_exist("customerX");

    cerr << ec.delete_index("customer");
    cerr << ec.delete_index("customerX");

 //    es.pretty(true);
//    auto r1 = es.get("/cycling_business/brand/_search", 3, body1);
//    std::cout << r1->data() << std::endl;

//    cpaf::streams::to_file(std::string("/tmp/_records.json"), r1->data());
//    auto r2 = es.get("/cycling_*/_search", 1, body2);
//    std::cout << r2->data() << std::endl;
//    cpaf::streams::to_file(std::string("/tmp/_es_all_records.json"), r2->data());

//    std::cout << es.read_record("/customer/external/1") << std::endl;

//    const std::string customer1 = R"(
//    {
//      "name": "John Dalton"
//    }
//    )";

//    es.write_record("/customer/external/1", customer1);
//    std::cout << es.read_record("/customer/external/1") << std::endl;
    return 0;
}


//////////////////////////

int main1()
{

    cpaf::elasticsearch::es_connection es;

//    const string url = "http://httpbin.org:80/get";
    const string url = "http://localhost:9200/_search";
//    const string url = "localhost:9200/cycling_business/_search' ";
    const string path = "/tmp/download.html";

//    curl -XGET 'localhost:9200/cycling_business*/_search?pretty' -H 'Content-Type: application/json'
//    WEBCC_LOG_INIT("", webcc::LOG_CONSOLE);

//    webcc::ClientSession sess;
//    webcc::ClientSession& session = sess;
    webcc::ClientSession& session = es.session();
//    session.SetHeader("Content-Type", "application/json");
//    session.set_ssl_verify(false);

    const std::string body = R"(
                             {
                                 "query" : {
                                     "term" : { "shop_id" : "jupiter.dk" }
                                 }
                             }
                             )";
    const std::string body1 = R"(
                             {
                                 "query" : {
                                     "match_all" : {}
                                 }
                             }
                             )";


//    webcc::RequestBuilder& request = es.request();
    es.pretty(true);
    webcc::RequestBuilder& request = es.getb("/cycling_*/_search", 2, body);
//    request.Get("http://localhost:9200/cycling_*/_search");
//    request.Query("pretty", "1");
//    request.Header("Accept", "application/json");
//    request.Query("size", "1");
//    request.Body(body);

//    std::unique_ptr<webcc::RequestBuilder> request_ptr = std::make_unique<webcc::RequestBuilder>();
//    request_ptr->Get("http://localhost:9200/cycling_business/brand/_search");
//    request_ptr->Header("Accept", "application/json");
//    request_ptr->Body(body1);
//    request_ptr->Query("pretty", "1");
//    request_ptr->Query("size", "1");


//    auto request = (webcc::RequestBuilder{}
//                                  .Get("http://localhost:9200/cycling_*/_search")
//                                  .Header("Accept", "application/json")
//                                  .Query("pretty", "1")
//                                  .Body(body)
//                                    );
//    auto r = session.Send(request());

//    auto r = session.Send(request
//                ()
//                );
    auto r = session.Send((request)());
    std::cout << r->data() << std::endl;

    return 0;
}


