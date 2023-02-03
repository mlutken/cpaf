#ifndef CPAF_STL_UTILS_H
#define CPAF_STL_UTILS_H

#include <vector>
#include <algorithm>
#include <nlohmann/json.hpp>
#include <cpaf_libs/utils/cpaf_json_utils.h>

namespace cpaf
{
	template <typename CONTAINER, typename ELEM>
	typename CONTAINER::const_iterator  find_in ( const CONTAINER& container, const ELEM& elem )
	{
		typename CONTAINER::const_iterator it;
		it = find ( container.begin(), container.end(), elem );
		return it;
	}

	template <typename CONTAINER, typename ELEM>
	typename CONTAINER::iterator  find_in ( CONTAINER& container, const ELEM& elem )
	{
		typename CONTAINER::iterator it;
		it = find ( container.begin(), container.end(), elem );
		return it;
	}

    template <typename CONTAINER, typename ELEM>
    bool contained_in ( const CONTAINER& container, const ELEM& elem )
    {
        return find_in(container, elem) != container.end();
    }

    template <typename CONTAINER, typename ELEM>
    bool push_back_unique (CONTAINER& container, const ELEM& elem)
    {
        if (!contained_in (container, elem)) {
            container.push_back(elem);
            return true;
        }
        return false;
    }

	template <typename CONTAINER, typename ELEM>
    bool contained_in_find ( const CONTAINER& container, const ELEM& elem )
	{
        return find_in(container, elem) != container.end();
	}

	template <typename CONTAINER, typename ELEM>
    bool remove_from ( CONTAINER& container, const ELEM& elem )
	{
		typename CONTAINER::iterator it = find_in ( container, elem );
		bool bDidRemoveElem =  it != container.end();
		if ( bDidRemoveElem ) container.erase ( it );
		return bDidRemoveElem;
	}

	template <typename CONTAINER_SRC, typename CONTAINER_DEST>
	void assign_inserter ( const CONTAINER_SRC& src, CONTAINER_DEST& dst )
	{
		std::copy(src.begin(),src.end(), std::inserter(dst, dst.begin())  );
	}

    template <template<class,class,class...> class C, typename V, typename K, typename... Args>
    V map_get(const C<K,V,Args...>& map_container, K const& key, const V& default_val)
    {
        typename C<K,V,Args...>::const_iterator it = map_container.find( key );
        if (it == map_container.end())
            return default_val;
        return it->second;
    }

    template <template<class,class,class...> class C, typename V, typename K, typename... Args>
    V map_find(const C<K,V,Args...>& map_container, const V& default_val, K const& key)
    {
        typename C<K,V,Args...>::const_iterator it = map_container.find( key );
        if (it == map_container.end())
            return default_val;
        return it->second;
    }

//    template <typename MAP_CONTAINER, typename KEY_TYPE, typename VALUE_TYPE>
//    VALUE_TYPE map_find (const MAP_CONTAINER& map_container, const VALUE_TYPE& default_val, const KEY_TYPE& key )
//    {
//        const auto it = map_container.find ( key );
//        if ( it != map_container.end() )    return it->second;
//        else                            return default_val;
//    }

    inline std::string map_find ( const nlohmann::json& container,
                                  const std::string& default_val,
                                  const std::string& key )
    {
        const auto it = container.find ( key );
        if ( it != container.end() )    return it->get<std::string>();
        else                            return default_val;
    }

    inline float map_find ( const nlohmann::json& container,
                            float default_val,
                            const std::string& key )
    {
        return json_value_float(container, key, default_val);
    }



} //end namespace cpaf


#endif //CPAF_STL_UTILS_H

