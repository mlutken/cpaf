#ifndef CPAF_EXCHANGE_RATES_H
#define CPAF_EXCHANGE_RATES_H

#include <boost/filesystem/path.hpp>
#include <nlohmann/json.hpp>

namespace cpaf {

/** Simple class to handle excahnge rates.
@todo Since we depend on cpaf_streams, this class needs to be moved to a
different library/folder, i.e. - not stay here in cpaf_libs/utils. We might
need a new cpaf_libs/subfolder; just need a good name/place. */
class exchange_rates
{
public:
    exchange_rates();
    float       convert             (const std::string& from_currency_code,
                                     const std::string& to_currency_code,
                                     float amount) const;
    float       to_eur              (const std::string& from_currency_code,
                                     float amount) const;
    float       from_eur            (const std::string& to_currency_code,
                                     float amount_in_eur) const;

    void        set_exchange_rates  (const boost::filesystem::path& exchange_rates_file);
    void        set_exchange_rates  (const nlohmann::json& exchange_rates_jo);

private:
    nlohmann::json      from_eur_;
    nlohmann::json      to_eur_;
};

} // END namespace cpaf

#endif //CPAF_EXCHANGE_RATES_H

