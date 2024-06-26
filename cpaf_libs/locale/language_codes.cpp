
#include "language_codes.h"
#include <algorithm>
#include <iostream>

#include <cpaf_libs/locale/translator.h>
#include <cpaf_libs/utils/cpaf_json_utils.h>

using namespace std;
using namespace nlohmann;

namespace cpaf::locale {

string language_codes::language_name(const std::string& iso639_2_language_code, const translator& tr)
{
    return tr.tr(language_name(iso639_2_language_code));
}


string language_codes::language_name(const std::string& iso639_2_language_code)
{
    return cpaf::json_value_str(codes_and_names(), iso639_2_language_code, iso639_2_language_code);
}


bool language_codes::is_languge_code(const std::string& iso639_2_language_code)
{
    return codes_and_names().contains(iso639_2_language_code);
}


const nlohmann::json& language_codes::codes_and_names()
{
    const static nlohmann::json data = nlohmann::json::parse(
        u8R"delimiter(
    {
        "aa": "Afar",
        "ab": "Abkhazian",
        "ae": "Avestan",
        "af": "Afrikaans",
        "ak": "Akan",
        "am": "Amharic",
        "an": "Aragonese",
        "ar": "Arabic",
        "ar-ae": "Arabic (U.A.E.)",
        "ar-bh": "Arabic (Bahrain)",
        "ar-dz": "Arabic (Algeria)",
        "ar-eg": "Arabic (Egypt)",
        "ar-iq": "Arabic (Iraq)",
        "ar-jo": "Arabic (Jordan)",
        "ar-kw": "Arabic (Kuwait)",
        "ar-lb": "Arabic (Lebanon)",
        "ar-ly": "Arabic (Libya)",
        "ar-ma": "Arabic (Morocco)",
        "ar-om": "Arabic (Oman)",
        "ar-qa": "Arabic (Qatar)",
        "ar-sa": "Arabic (Saudi Arabia)",
        "ar-sy": "Arabic (Syria)",
        "ar-tn": "Arabic (Tunisia)",
        "ar-ye": "Arabic (Yemen)",
        "as": "Assamese",
        "av": "Avaric",
        "ay": "Aymara",
        "az": "Azeri",
        "ba": "Bashkir",
        "be": "Belarusian",
        "bg": "Bulgarian",
        "bh": "Bihari",
        "bi": "Bislama",
        "bm": "Bambara",
        "bn": "Bengali",
        "bo": "Tibetan",
        "br": "Breton",
        "bs": "Bosnian",
        "ca": "Catalan",
        "ce": "Chechen",
        "ch": "Chamorro",
        "co": "Corsican",
        "cr": "Cree",
        "cs": "Czech",
        "cu": "Church Slavonic",
        "cv": "Chuvash",
        "cy": "Welsh",
        "da": "Danish",
        "de": "German",
        "de-at": "German (Austria)",
        "de-ch": "German (Switzerland)",
        "de-de": "German (Germany)",
        "de-li": "German (Liechtenstein)",
        "de-lu": "German (Luxembourg)",
        "div": "Divehi",
        "dv": "Divehi",
        "dz": "Bhutani",
        "ee": "Ewe",
        "el": "Greek",
        "en": "English",
        "en-au": "English (Australia)",
        "en-bz": "English (Belize)",
        "en-ca": "English (Canada)",
        "en-cb": "English (Caribbean)",
        "en-gb": "English (United Kingdom)",
        "en-ie": "English (Ireland)",
        "en-jm": "English (Jamaica)",
        "en-nz": "English (New Zealand)",
        "en-ph": "English (Philippines)",
        "en-tt": "English (Trinidad and Tobago)",
        "en-us": "English (United States)",
        "en-za": "English (South Africa)",
        "en-zw": "English (Zimbabwe)",
        "eo": "Esperanto",
        "es": "Spanish",
        "es-ar": "Spanish (Argentina)",
        "es-bo": "Spanish (Bolivia)",
        "es-cl": "Spanish (Chile)",
        "es-co": "Spanish (Colombia)",
        "es-cr": "Spanish (Costa Rica)",
        "es-do": "Spanish (Dominican Republic)",
        "es-ec": "Spanish (Ecuador)",
        "es-es": "Spanish (Spain)",
        "es-gt": "Spanish (Guatemala)",
        "es-hn": "Spanish (Honduras)",
        "es-mx": "Spanish (Mexico)",
        "es-ni": "Spanish (Nicaragua)",
        "es-pa": "Spanish (Panama)",
        "es-pe": "Spanish (Peru)",
        "es-pr": "Spanish (Puerto Rico)",
        "es-py": "Spanish (Paraguay)",
        "es-sv": "Spanish (El Salvador)",
        "es-us": "Spanish (United States)",
        "es-uy": "Spanish (Uruguay)",
        "es-ve": "Spanish (Venezuela)",
        "et": "Estonian",
        "eu": "Basque",
        "fa": "Farsi",
        "ff": "Fulah",
        "fi": "Finnish",
        "fj": "Fiji",
        "fo": "Faroese",
        "fr": "French",
        "fr-be": "French (Belgium)",
        "fr-ca": "French (Canada)",
        "fr-ch": "French (Switzerland)",
        "fr-fr": "French (France)",
        "fr-lu": "French (Luxembourg)",
        "fr-mc": "French (Monaco)",
        "fy": "Frisian",
        "ga": "Irish",
        "gd": "Gaelic",
        "gl": "Galician",
        "gn": "Guarani",
        "gu": "Gujarati",
        "gv": "Manx",
        "ha": "Hausa",
        "he": "Hebrew",
        "hi": "Hindi",
        "ho": "Hiri Motu",
        "hr": "Croatian",
        "hr-ba": "Croatian (Bosnia and Herzegovina)",
        "hr-hr": "Croatian (Croatia)",
        "ht": "Haitian",
        "hu": "Hungarian",
        "hy": "Armenian",
        "hz": "Herero",
        "ia": "Interlingua",
        "id": "Indonesian",
        "ie": "Interlingue",
        "ig": "Igbo",
        "ii": "Sichuan Yi",
        "ik": "Inupiak",
        "in": "Indonesian",
        "io": "Ido",
        "is": "Icelandic",
        "it": "Italian",
        "it-ch": "Italian (Switzerland)",
        "it-it": "Italian (Italy)",
        "iu": "Inuktitut",
        "iw": "Hebrew",
        "ja": "Japanese",
        "ji": "Yiddish",
        "jv": "Javanese",
        "jw": "Javanese",
        "ka": "Georgian",
        "kg": "Kongo",
        "ki": "Kikuyu",
        "kj": "Kuanyama",
        "kk": "Kazakh",
        "kl": "Greenlandic",
        "km": "Cambodian",
        "kn": "Kannada",
        "ko": "Korean",
        "kok": "Konkani",
        "kr": "Kanuri",
        "ks": "Kashmiri",
        "ku": "Kurdish",
        "kv": "Komi",
        "kw": "Cornish",
        "ky": "Kirghiz",
        "kz": "Kyrgyz",
        "la": "Latin",
        "lb": "Luxembourgish",
        "lg": "Ganda",
        "li": "Limburgan",
        "ln": "Lingala",
        "lo": "Laothian",
        "ls": "Slovenian",
        "lt": "Lithuanian",
        "lu": "Luba-Katanga",
        "lv": "Latvian",
        "mg": "Malagasy",
        "mh": "Marshallese",
        "mi": "Maori",
        "mk": "FYRO Macedonian",
        "ml": "Malayalam",
        "mn": "Mongolian",
        "mo": "Moldavian",
        "mr": "Marathi",
        "ms": "Malay",
        "ms-bn": "Malay (Brunei Darussalam)",
        "ms-my": "Malay (Malaysia)",
        "mt": "Maltese",
        "my": "Burmese",
        "na": "Nauru",
        "nb": "Norwegian (Bokmal)",
        "nd": "North Ndebele",
        "ne": "Nepali (India)",
        "ng": "Ndonga",
        "nl": "Dutch",
        "nl-be": "Dutch (Belgium)",
        "nl-nl": "Dutch (Netherlands)",
        "nn": "Norwegian (Nynorsk)",
        "no": "Norwegian",
        "nr": "South Ndebele",
        "ns": "Northern Sotho",
        "nv": "Navajo",
        "ny": "Chichewa",
        "oc": "Occitan",
        "oj": "Ojibwa",
        "om": "(Afan)/Oromoor/Oriya",
        "or": "Oriya",
        "os": "Ossetian",
        "pa": "Punjabi",
        "pi": "Pali",
        "pl": "Polish",
        "ps": "Pashto/Pushto",
        "pt": "Portuguese",
        "pt-br": "Portuguese (Brazil)",
        "pt-pt": "Portuguese (Portugal)",
        "qu": "Quechua",
        "qu-bo": "Quechua (Bolivia)",
        "qu-ec": "Quechua (Ecuador)",
        "qu-pe": "Quechua (Peru)",
        "rm": "Rhaeto-Romanic",
        "rn": "Kirundi",
        "ro": "Romanian",
        "ru": "Russian",
        "rw": "Kinyarwanda",
        "sa": "Sanskrit",
        "sb": "Sorbian",
        "sc": "Sardinian",
        "sd": "Sindhi",
        "se": "Sami",
        "se-fi": "Sami (Finland)",
        "se-no": "Sami (Norway)",
        "se-se": "Sami (Sweden)",
        "sg": "Sangro",
        "sh": "Serbo-Croatian",
        "si": "Singhalese",
        "sk": "Slovak",
        "sl": "Slovenian",
        "sm": "Samoan",
        "sn": "Shona",
        "so": "Somali",
        "sq": "Albanian",
        "sr": "Serbian",
        "sr-ba": "Serbian (Bosnia and Herzegovina)",
        "sr-sp": "Serbian (Serbia and Montenegro)",
        "ss": "Siswati",
        "st": "Sesotho",
        "su": "Sundanese",
        "sv": "Swedish",
        "sv-fi": "Swedish (Finland)",
        "sv-se": "Swedish (Sweden)",
        "sw": "Swahili",
        "sx": "Sutu",
        "syr": "Syriac",
        "ta": "Tamil",
        "te": "Telugu",
        "tg": "Tajik",
        "th": "Thai",
        "ti": "Tigrinya",
        "tk": "Turkmen",
        "tl": "Tagalog",
        "tn": "Tswana",
        "to": "Tonga",
        "tr": "Turkish",
        "ts": "Tsonga",
        "tt": "Tatar",
        "tw": "Twi",
        "ty": "Tahitian",
        "ug": "Uighur",
        "uk": "Ukrainian",
        "ur": "Urdu",
        "us": "English",
        "uz": "Uzbek",
        "ve": "Venda",
        "vi": "Vietnamese",
        "vo": "Volapuk",
        "wa": "Walloon",
        "wo": "Wolof",
        "xh": "Xhosa",
        "yi": "Yiddish",
        "yo": "Yoruba",
        "za": "Zhuang",
        "zh": "Chinese",
        "zh-cn": "Chinese (China)",
        "zh-hk": "Chinese (Hong Kong SAR)",
        "zh-mo": "Chinese (Macau SAR)",
        "zh-sg": "Chinese (Singapore)",
        "zh-tw": "Chinese (Taiwan)",
        "zu": "Zulu"
    }
    )delimiter");

    return data;
}


/**
 *  @see https://en.wikipedia.org/wiki/List_of_ISO_639_language_codes
 *  */
std::string language_codes::iso639_3_to_2(const std::string& iso639_3) {
    if (iso639_3.size() == 2) {
        return iso639_3;
    }
    static const std::unordered_map<std::string, std::string> iso639_3_to_2 = {
        {"afr", "af"}, // Afrikaans
        {"alb", "sq"}, // Albanian
        {"sqi", "sq"}, // Albanian
        {"bur", "my"}, // Burmese
        {"mya", "my"}, // Burmese
        {"eng", "en"}, // English
        {"fre", "fr"}, // French
        {"fra", "fr"}, // French
        {"deu", "de"}, // German
        {"spa", "es"}, // Spanish
        {"ita", "it"}, // Italian
        {"isl", "is"}, // Icelandic
        {"ice", "is"}, // Icelandic
        {"por", "pt"}, // Portuguese
        {"nld", "nl"}, // Dutch
        {"swe", "sv"}, // Swedish
        {"dan", "da"}, // Danish
        {"nor", "no"}, // Norwegian
        {"fin", "fi"}, // Finnish
        {"ell", "el"}, // Greek
        {"tur", "tr"}, // Turkish
        {"rus", "ru"}, // Russian
        {"pol", "pl"}, // Polish
        {"ces", "cs"}, // Czech
        {"hun", "hu"}, // Hungarian
        {"slk", "sk"}, // Slovak
        {"hrv", "hr"}, // Croatian
        {"ron", "ro"}, // Romanian
        {"bul", "bg"}, // Bulgarian
        {"srp", "sr"}, // Serbian
        {"ukr", "uk"}, // Ukrainian
        {"lit", "lt"}, // Lithuanian
        {"slv", "sl"}, // Slovenian
        {"lav", "lv"}, // Latvian
        {"est", "et"}, // Estonian
        {"ara", "ar"}, // Arabic
        {"heb", "he"}, // Hebrew
        {"hin", "hi"}, // Hindi
        {"ben", "bn"}, // Bengali
        {"urd", "ur"}, // Urdu
        {"pan", "pa"}, // Punjabi
        {"tam", "ta"}, // Tamil
        {"tel", "te"}, // Telugu
        {"guj", "gu"}, // Gujarati
        {"kan", "kn"}, // Kannada
        {"mal", "ml"}, // Malayalam
        {"tha", "th"}, // Thai
        {"ind", "id"}, // Indonesian
        {"msa", "ms"}, // Malay
        {"fil", "fil"}, // Filipino
        {"vie", "vi"}, // Vietnamese
        {"jpn", "ja"}, // Japanese
        {"kor", "ko"}, // Korean
        {"chi", "zh"}, // Chinese
        {"zho", "zh"}, // Chinese
    };

    auto it = iso639_3_to_2.find(iso639_3);
    if (it != iso639_3_to_2.end()) {
        return it->second;
    } else {
        // If not found, return empty string or handle the case accordingly
        std::cerr << "LOG_WARN: iso639_3_to_2, language code '" << iso639_3 << "' not known\n";
        return "";
    }
}

} //END namespace cpaf::locale
