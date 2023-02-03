
#include <iomanip>
#include <streams/cpaf_streams.h>
#include <containers/cpaf_string8.h>
#include "cpaf_json_streams.h"


namespace fs = boost::filesystem;
namespace cs = cpaf::streams;
using json = nlohmann::json;

namespace cpaf::streams {

void to_file(const std::string& resource_path, const nlohmann::json& jobject, int indentation)
{
    auto os = create_ostream(resource_path);
    os.os() << std::setw(indentation) << jobject;
}

void to_file(const boost::filesystem::path& file_path, const nlohmann::json& jobject, int indentation)
{
    to_file(file_path.string(), jobject, indentation);
}

auto to_json(const std::string& resource_path,
                       const nlohmann::json& default_jo) -> nlohmann::json
{
    if (!fs::exists(resource_path)) {
        return default_jo;
    }
    auto str8 = cs::to_string8(resource_path);
    try {
        const auto jo = json::parse(str8);
        if (jo.is_null()) {
            return default_jo;
        }
        else {
            return jo;
        }
    }  catch (...) {
    }
    return default_jo;
}

auto to_json(const boost::filesystem::path& file_path,
                       const nlohmann::json& default_jo) -> nlohmann::json
{
    return to_json(file_path.string(), default_jo);
}


} //END namespace cpaf::streams


