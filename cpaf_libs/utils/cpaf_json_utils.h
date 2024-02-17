#pragma once
#include <string>
#include <map>
#include <vector>
#include <optional>
#include <nlohmann/json.hpp>

namespace cpaf {
using json_vec              = std::vector<nlohmann::json>;
using json_ptr_vec          = std::vector<nlohmann::json*>;
using json_const_ptr_vec    = std::vector<const nlohmann::json*>;

enum class json_pretty { no, yes };

std::string     simple_json_to_string   (const nlohmann::json& object);
std::string     to_string               (const nlohmann::json& object, std::string default_value);
bool            parse_non_strict        (nlohmann::json& json_object, const std::string& str);
nlohmann::json  parse_non_strict        (const std::string& str);
nlohmann::json  json_parse_safe         (const std::string& json_str, const nlohmann::json& default_obj = nlohmann::json::object());
nlohmann::json  json_parse_safe         (const std::string& json_str, const std::string& default_obj_json_str);
float           json_value_float        (const nlohmann::json& jo, float default_value);
float           json_value_float        (const nlohmann::json& jo_object, const std::string& key, float default_value);
int32_t         json_value_int32        (const nlohmann::json& jo, int32_t default_value);
int32_t         json_value_int32        (const nlohmann::json& jo_object, const std::string& key, int32_t default_value);
std::string     json_value_str          (const nlohmann::json& jo, const std::string&  default_value);
std::string     json_value_str          (const nlohmann::json& jo_object, const std::string& key, const std::string& default_value);
nlohmann::json  json_value_object       (const nlohmann::json& jo_object, const std::string& key, const nlohmann::json& default_value = nlohmann::json::object());
void            json_remove_fields      (nlohmann::json& jo, const std::vector<std::string>& fields_to_remove);
nlohmann::json  json_copy_fields        (const nlohmann::json& jo, const std::vector<std::string>& fields_to_copy);
nlohmann::json  json_copy_fields        (const nlohmann::json& jo, const nlohmann::json& fields_to_copy);


/**
Convert a json list object to a vector type.

For example:
@code
auto vec = json_to_vec<std::vector<std::string>>(my_json_object);
@endcode
*/
template <class VEC_TYPE>
VEC_TYPE json_to_vec(const nlohmann::json& jo, size_t size_hint = 8)
{
    VEC_TYPE v;
    v.reserve(size_hint);
    for (const auto& je : jo.items()) {
        v.push_back(je.value());
    }
    return v;
}

/**
Append a json list object to a vector type.

For example:
@code
std::vector<std::string> v;
json_append_to_vec(v, my_json_object);
@endcode
*/
template <class VEC_TYPE>
void json_append_to_vec(VEC_TYPE& vec, const nlohmann::json& jo)
{
    for (const auto& je : jo.items()) {
        vec.push_back(je.value());
    }
}


/**
Convert a map-like json object to another map type.

For example:
@code
auto map = json_to_map<std::map<std::string, int>>(my_json_object);
@endcode
*/
template <class MAP_TYPE>
MAP_TYPE json_to_map(const nlohmann::json& jo)
{
    MAP_TYPE map;
    for (const auto& [key, val]: jo.items()) {
        map.emplace(key, val);
    }
    return map;
}

/**
Convert a map-like json object to an std::map.

For example:
@code
auto map = json_to_std_map<std::string, int>(my_json_object);
@endcode
*/
template <typename KEY, typename VALUE>
std::map<KEY, VALUE> json_to_std_map(const nlohmann::json& jo)
{
    return json_to_map<std::map<KEY, VALUE>>(jo);
}


inline std::string map_find ( const nlohmann::json& container,
                            const std::string& key,
                            const std::string& default_val)
{
    const auto it = container.find ( key );
    if ( it != container.end() )    return it->get<std::string>();
    else                            return default_val;
}

inline float map_find ( const nlohmann::json& container,
                        const std::string& key,
                        float default_val
                      )
{
    return json_value_float(container, key, default_val);
}

} // END namespace cpaf

