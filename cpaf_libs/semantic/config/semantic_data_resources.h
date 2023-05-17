#ifndef CPAF_SEMANTIC_DATA_SOURCES_H
#define CPAF_SEMANTIC_DATA_SOURCES_H

#include <string>
#include <vector>
#include <filesystem>

namespace cpaf::semantic
{

/** Holds paths/URIs to sematic classification data sources.

In practice this simply means we hold a list of files/directories or
perhaps database references to resources containing sematic clasification data.

For example a snippet from a file with classification data could look like this:
@code
{
    "default_weights" : {
        "*" : "21",
        "item_title" : "40",
        "description_text" : "8",
        "breadcrumbs_text" : "20"
    },
    "values":
    [
        {
            "val": "forks_component",
            "component_group": "forks_and_shocks_group",
            "type": "component_type",
            "names_en": ["fork"],
            "names_da": ["forgaffel", "forgafler"]
        }
        ,
        {
            "default_weights" : {
                "description_text" : "1"
            },
            "val": "frames_component",
            "component_group": "frames_group",
            "type": "component_type",
            "names_en": ["frame", "frames"],
            "names_da": ["[item_title:60] racer ramme", "rammer", "stel", "racerstel"]
        },
        {
            "component_group": "cleaning_and_maintenance_group",
            "display_da": "pudse- og smøremidler",
            "display_en": "polish and lubricant",
            "names_da": [
                "forgaffel beskyttelses dynamic",
                "forgaffel olie",
                "voks"
            ],
            "names_en": [
            ],
            "type": "component_type",
            "val": "polish_and_lubricant_component"
        }

    ]
}
@endcode
*/
class semantic_data_resources {
public:
    using local_files_vec               = std::vector<std::filesystem::path>;
    using local_files_const_iterator    = local_files_vec::const_iterator;

    void            clear                   ();
    void            add_excluded_file_name  (const std::string& name);
    void            add_resource            (const std::string& resource_path);
    void            add_resource            (const std::filesystem::path& file_path);
    void            add_resource            (const std::string& resource_path, const std::string& pattern);
    void            add_resource            (const std::filesystem::path& file_path, const std::string& pattern);
    void            init                    ();

    const local_files_vec&      local_files () const { return all_local_files_; }

private:
    void                        add_local_resource  (const std::string& resource_path, const std::string& pattern);
    void                        add_local_file      (const std::filesystem::path& file_path);
    bool                        file_is_excluded    (const std::filesystem::path& file_path) const;

    std::vector<std::string>    original_resources_;    ///< This holds the "original" paths as added using add_resource()
    std::vector<std::string>    excluded_matches_;      ///< List of excluded filenames
    local_files_vec             all_local_files_;       ///< This holds all local files as expanded from directory resource paths and plain file resource paths
};

} //end namespace cpaf::semantic

#endif //CPAF_SEMANTIC_DATA_FILES_H
