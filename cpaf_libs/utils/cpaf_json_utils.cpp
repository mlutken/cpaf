#include "cpaf_json_utils.h"

#include <string>
#include <cpaf_libs/utils/cpaf_string_utils.hpp>

namespace cpaf
{

/** Converts an json object to a string representation.
Similar to json.dump() but without the quotes.
Assumes that the object is not structured (ie. primitive).
For structured object a empty string is returned.

Special values like null, numbers etc. are converted to
string representation.
*/
std::string simple_json_to_string(const nlohmann::json& object)
{
    if (object.is_structured()) return "";
    if (object.is_null()) return "null";
    else if (object.is_string()) return object.get<std::string>();
    else if (object.is_boolean()) return object.get<bool>() ? "true" : "false";
    else if (object.is_number_float()) return std::to_string(object.get<double>());
    else if (object.is_number_integer()) return std::to_string(object.get<long>());
    else if (object.is_number_unsigned()) return std::to_string(object.get<unsigned long>()); // Seems unused
    else if (object.is_number()) return std::to_string(object.get<double>()); // Seems unused
    else {
        // Fallback in case we forgot an if above!
        std::string s = object.dump();
        return s.substr(1, s.length() -2);
    }
}

/** Converts an json object to a string representation.
Similar to json.dump() but without the quotes and with a default value
Assumes that the object is not structured (ie. primitive).
For structured object a empty string is returned.

Special values like null, numbers etc. are converted to
string representation.
*/
std::string to_string(const nlohmann::json& object, std::string default_value)
{
    std::string value;
    if (object.is_null()) return default_value;
    else if (object.is_string()         ) value = object.get<std::string>();
    else if (object.is_boolean()        ) value = object.get<bool>() ? "true" : "false";
    else if (object.is_number_float()   ) value = std::to_string(object.get<double>());
    else if (object.is_number_integer() ) value = std::to_string(object.get<long>());
    else if (object.is_number_unsigned()) value = std::to_string(object.get<unsigned long>()); // Seems unused
    else if (object.is_number()         ) value = std::to_string(object.get<double>()); // Seems unused
    else {
        // if (object.is_structured());
        // Fallback in case we forgot an if above!
        std::string s = object.dump();
        value = s.substr(1, s.length() -2);
    }
    return value;
}


bool parse_non_strict(nlohmann::json& json_object, const std::string& str)
{
    nlohmann::detail::json_sax_dom_parser<nlohmann::json> sdp(json_object, true);
    return nlohmann::json::sax_parse(str, &sdp, nlohmann::detail::input_format_t::json, false);
}

nlohmann::json parse_non_strict(const std::string& str)
{
    nlohmann::json json_object;
    if (parse_non_strict(json_object, str)) {
        return json_object;
    }
    return nlohmann::json();
}

nlohmann::json json_parse_safe(const std::string& json_str, const nlohmann::json& default_obj)
{
    try {
        return nlohmann::json::parse(json_str);
    } catch (...) {
    }
    return default_obj;
}

nlohmann::json json_parse_safe(const std::string& json_str, const std::string& default_obj_json_str)
{
    nlohmann::json rec;
    try {
        rec = nlohmann::json::parse(json_str);
    } catch (...) {
        rec = nlohmann::json::parse(default_obj_json_str);
    }
    return rec;
}

float json_value_float(const nlohmann::json& jo, float default_value)
{
    if (jo.is_number()) {
        return jo.get<float>();
    }
    else if (jo.is_string()) {
        return cpaf::to_float_default(jo.get<std::string>(), default_value);
    }
    return default_value;
}

float json_value_float(const nlohmann::json& jo_object,
                       const std::string& key,
                       float default_value)
{
    if (jo_object.is_object()) {
        const auto elem = jo_object.value(key, nlohmann::json());
        return json_value_float(elem, default_value);
    }
    return default_value;
}

int32_t json_value_int32(const nlohmann::json& jo,
                         int32_t default_value)
{
    if (jo.is_number_integer()) {
        return jo.get<int32_t>();
    }
    else if (jo.is_number()) {
        return static_cast<int32_t>(jo.get<float>());
    }
    else if (jo.is_string()) {
        return cpaf::to_int_default(jo.get<std::string>(), default_value);
    }
    return default_value;
}

int32_t json_value_int32(const nlohmann::json& jo_object,
                         const std::string& key,
                         int32_t default_value)
{
    if (jo_object.is_object()) {
        const auto elem = jo_object.value(key, nlohmann::json());
        return json_value_int32(elem, default_value);
    }
    return default_value;
}


std::string json_value_str(const nlohmann::json& jo, const std::string& default_value)
{
    return to_string(jo, default_value);
}


std::string json_value_str(const nlohmann::json& jo_object,
                           const std::string& key,
                           const std::string& default_value)
{
    if (jo_object.is_object()) {
        const auto elem = jo_object.value(key, nlohmann::json());
        return json_value_str(elem, default_value);
    }
    return default_value;
}

nlohmann::json json_value_object(const nlohmann::json& jo_object,
                                 const std::string& key,
                                 const nlohmann::json& default_value)
{
    if (jo_object.is_object()) {
        const auto elem = jo_object.value(key, nlohmann::json::object());
        if (elem.is_null()) return default_value;
        return elem;
    }
    return default_value;
}

nlohmann::json json_value_array(const nlohmann::json& jo_object,
                                const std::string& key,
                                const nlohmann::json& default_value)
{
    if (jo_object.is_object()) {
        const auto elem = jo_object.value(key, nlohmann::json::object());
        if (elem.is_null()) return default_value;
        if (!elem.is_array()) return default_value;
        return elem;
    }
    return default_value;
}


/** Remove specified fields from json object.
*/
void json_remove_fields (nlohmann::json& jo,
                         const std::vector<std::string>& fields_to_remove)
{
    for (const auto& field : fields_to_remove) {
        jo.erase(field);
    }
}

nlohmann::json json_copy_fields (const nlohmann::json& jo,
                                 const std::vector<std::string>& fields_to_copy)
{
    nlohmann::json jo_ret;
    for (const auto& field: fields_to_copy) {
        const auto it = jo.find(field);
        if (it != jo.end()) {
            jo_ret[field] = *it;
        }
    }
    return jo_ret;
}

nlohmann::json json_copy_fields (const nlohmann::json& jo,
                                 const nlohmann::json& fields_to_copy)
{
    nlohmann::json jo_ret;
    for (const auto& el: fields_to_copy.items()) {
        const std::string& field = el.value();
        const auto it = jo.find(field);
        if (it != jo.end()) {
            jo_ret[field] = *it;
        }
    }
    return jo_ret;
}

} // END namespace cpaf

