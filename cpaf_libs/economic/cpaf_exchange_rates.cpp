#include "cpaf_exchange_rates.h"

#include <boost/filesystem/operations.hpp>
#include <cpaf_libs/streams/cpaf_json_streams.h>

namespace cpaf
{

exchange_rates::exchange_rates()
{
    to_eur_["eur"] = 1.0;
    from_eur_["eur"] = 1.0;
}

float exchange_rates::convert(const std::string& from_currency_code, const std::string& to_currency_code, float amount) const
{
    return from_eur(to_currency_code, to_eur(from_currency_code, amount));
}

float exchange_rates::to_eur(const std::string& from_currency_code, float amount) const
{
    return to_eur_[from_currency_code].get<float>() * amount;
}

float exchange_rates::from_eur(const std::string& to_currency_code, float amount_in_eur) const
{
    return from_eur_[to_currency_code].get<float>() * amount_in_eur;
}

void exchange_rates::set_exchange_rates(const boost::filesystem::path& exchange_rates_file)
{
    if (boost::filesystem::exists(exchange_rates_file)) {
        set_exchange_rates(streams::to_json(exchange_rates_file));
    }
}

void exchange_rates::set_exchange_rates(const nlohmann::json& exchange_rates_jo)
{
    to_eur_ = exchange_rates_jo["to_eur"];
    from_eur_ = exchange_rates_jo["from_eur"];
}


} // END namespace cpaf

