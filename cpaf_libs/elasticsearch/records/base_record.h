#ifndef CPAF_ELASTICSEARCH_BASE_RECORD_H
#define CPAF_ELASTICSEARCH_BASE_RECORD_H

#include <string>


namespace cpaf::elasticsearch
{
class fragment_node;

// -----------------
// --- attribute ---
// -----------------
/**
Base class for elastic search records.
From this we derive currently two concrete records
namely:
 - simple_record: Which is a "flat" record with one level key, value interface
 - json_record: Which can handle arbitrarely complex nested fields
*/
class base_record
{
public:
    virtual ~base_record() = default;

    // ----------------------------------
    // --- PUBLIC: Setter and getters ---
    // ----------------------------------
    const std::string&  string_data     () const                        { return do_string_data(); }
    void                string_data     (const std::string& rec_data)   { do_string_data(rec_data);}

private:
    virtual const std::string&  do_string_data  () const                        = 0;
    virtual void                do_string_data  (const std::string& rec_data)   = 0;

};




} //end namespace cpaf::elasticsearch


#endif //CPAF_ELASTICSEARCH_BASE_RECORD_H

