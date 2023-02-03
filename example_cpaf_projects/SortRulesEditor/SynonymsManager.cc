#include <vdbclient/SynonymsManager.h>

#include <QRegExp>

#include <algorithm>
using namespace std;
using namespace evbase;


namespace vdbclient {

const QString SynonymsManager::m_default_preferences_path = "/CAM/General 2D CAM/Synonyms/";
const SynonymsManager::SynonymWordsVec SynonymsManager::emptySynonymWordsVec = SynonymsManager::SynonymWordsVec{};


SynonymsManager::SynonymsManager()
{
}

/**
C++11 removes the need for manual locking. Concurrent execution shall wait if a static local variable is already being initialized.
§6.7 [stmt.dcl] p4
If control enters the declaration concurrently while the variable is being initialized, the concurrent
execution shall wait for completion of the initialization.

*/
SynonymsManager& SynonymsManager::instance()
{
    static SynonymsManager instance;
    return instance;
}


/** Test if given word is a synonym. Normally we assume a word represents a synonym
if the firs char is dollar-sign '$'.
\note If we at a later point want to give the user of this clas control over how a synonym is
 identified, we could make this a regular memeber function, and supply a setter function that
takes a std::function that we could call to determine whether a given word is a synonym.*/
bool SynonymsManager::isSynonymString(const QString &string_id)
{
    if (string_id.length() < 2) return false;
    return '$' == string_id[0];
}

QString SynonymsManager::synonymStringToSynonym(const QString &string_id)
{
    return string_id.mid(1);
}


/** Returns a reference to the element at specified location pos. No bounds checking is performed. */
SynonymsManager::Synonym& SynonymsManager::operator[](size_t i)
{
    return m_synonyms[i];
}

/** Returns a const reference to the element at specified location pos. No bounds checking is performed. */
const SynonymsManager::Synonym& SynonymsManager::operator[](size_t i) const
{
    return m_synonyms[i];
}

/** Returns a pointer to the Synonym at specified location pos. Bounds checking is performed
so that in case the position is invalind a null pointer is returned. */
SynonymsManager::Synonym *SynonymsManager::at(size_t i)
{
    if (i < size())
        return &m_synonyms[i];
    return nullptr;
}

/** Returns a pointer to the Synonym at specified location pos. Bounds checking is performed
so that in case the position is invalind a null pointer is returned. */
const SynonymsManager::Synonym *SynonymsManager::at(size_t i) const
{
    if (i < size())
        return &m_synonyms[i];
    return nullptr;
}

SynonymsManager::iterator SynonymsManager::find(const QString &synonym_list_name)
{
    auto it_end = end();
    auto it_lower = std::lower_bound(begin(), it_end, Synonym(synonym_list_name));

    if ( (it_lower != it_end) && (it_lower->m_synonym_list_name == synonym_list_name))
        return it_lower;

    return it_end;
}

SynonymsManager::const_iterator SynonymsManager::find(const QString &synonym_list_name) const
{
    const auto it_end = end();
    auto it_lower = std::lower_bound(begin(), it_end, Synonym(synonym_list_name));

    if ( (it_lower != it_end) && (it_lower->m_synonym_list_name == synonym_list_name))
        return it_lower;

    return it_end;
}


/** Store all synonyms in preferences.
\param preferences The preferences instance to store in. If null we use the default.
\param path Path in preferences where the synonyms are stored. */
void SynonymsManager::storeInPreferences(Preferences */*preferences*/, QString /*path*/)
{
//    if (path.isEmpty())
//        path = m_default_preferences_path;

//    sortSynonyms();
//    if (!preferences)
//        preferences = PreferencesSingleton::get().get();

//    if (!preferences)
//        return;

//    preferences->remove(path.toUtf8().data(), true);
//    for (const Synonym& synonym : m_synonyms) {
//        std::string key = (path+synonym.m_synonym_list_name).toUtf8().data();
//        std::string value = synonym.synonymWordsAsString().toUtf8().data();
//        preferences->putString(key, value);
//    }
}

/** Load all synonyms from preferences.
\param preferences The preferences instance to store in. If null we use the default.
\param path Path in preferences to load the synonyms from. */
void SynonymsManager::loadFromPreferences(Preferences */*preferences*/, QString /*path*/)
{
//    clear();
//    if (path.isEmpty())
//        path = m_default_preferences_path;

//    if (!preferences)
//        preferences = PreferencesSingleton::get().get();

//    if (!preferences)
//        return;

//    Preferences::StringMap map;
//    preferences->getSubTree(path.toUtf8().data(), map);
//    for ( std::pair<string, string> p : map) {
//        addSynonymList(QString::fromUtf8(p.first.c_str()), QString::fromUtf8(p.second.c_str()));
//    }

    sortSynonyms();
}

/** Inserts empty synonyms at the specified location in the container. */
void SynonymsManager::insert(iterator pos, size_t count, const SynonymsManager::Synonym &synonym)
{
    m_synonyms.insert(pos, count, synonym);
}

/** Removes specified element from the container. */
SynonymsManager::iterator SynonymsManager::erase(SynonymsManager::iterator it)
{
    return m_synonyms.erase(it);
}


/** Removes specified elements from the container. */
SynonymsManager::iterator SynonymsManager::erase(SynonymsManager::iterator first, SynonymsManager::iterator last)
{
    return m_synonyms.erase(first, last);
}


/** Add synonym to a given synonym synonym_list_name.
\param synonym_list_name The synonyms synonym_list_name. For example HEAD, ARM_LEFT, etc.
\param synonyms_comma_separated List with the synonyms (comma separated string).
       For example "skull, head, cranium".
\param groups_comma_separated An optional list of group names which this synonym_list_name
       should belong to (comma separated string). */
void SynonymsManager::addSynonymList(const QString &synonym_list_name, const QString &synonyms_comma_separated)
{
    SynonymWordsVec synonym_words = evbase::stringVectorFromQString(synonyms_comma_separated, ',', true);
    m_synonyms.push_back(Synonym(synonym_list_name, synonym_words));
}


/** Remove synonym list.
\param synonym_list_name Name of synonym list to remove. */
void SynonymsManager::removeSynonymList(const QString &synonym_list_name)
{
    auto it = find(synonym_list_name);
    if (it != end())
        m_synonyms.erase(it);
}


/** Get vector of all the synonyms that belong to a certain synonym_list_name.
\param synonym_list_name The synonyms synonym_list_name. For example HEAD, ARM_LEFT, etc. */
const SynonymsManager::SynonymWordsVec &SynonymsManager::synonymsForName(const QString &synonym_list_name) const
{
    const auto it = find(synonym_list_name);
    if (it != end())
        return it->m_synonym_words;

    return SynonymsManager::emptySynonymWordsVec;
}

/**
Searches all synonym lists for a given synonym and returns it's synonym_list_name.
\param synonym The synonym to search for.
\return The synonyms synonym_list_name (like eg. HEAD, ARM_LEFT, etc) if found,
        otherwise an empty string. */
//SynonymsManager::Synonym SynonymsManager::searchSynonym(const QString &/*synonym*/) const
//{
//    Synonym synonym;

//    return synonym;
//}


/** Sorts the synonyms so that we can use binary search, when looking them up.
Helper function mainly used by storeInPreferences() and loadFromPreferences().
\note This function is not called with every insert or remove!*/
void SynonymsManager::sortSynonyms()
{
    std::sort(m_synonyms.begin(), m_synonyms.end());
}


/**
Executes the given string test operation operator using \a search_in and \a subject.
If \a subject is in fact a synonym (begins with '$', see SynonymsManager::isSynonymString()), then
the test operation is performed against all synonyms, like in compareWithSynonymName().

\see compareWithSynonymName()
\param oper The test operation to use:  Op::Equals, Op::Contains, Op::StartsWith, Op::EndsWith, Op::RegexMatch, Op::AlwaysTrue, Op::AlwaysFalse, Op::NoOp.
\param search_in The string to search within.
\param synonym_list_name The synonym list to use. Ie. all synonyms in list are tried.
\param match_type Can be either Match::WholeWords or Match::SubString.
\param case_sensitivity Can be either Qt::CaseSensitive or Qt::CaseInsensitive.
\return True if a least one of the synonyms matches with the Operation used. */
bool SynonymsManager::compare(SynonymsManager::Op oper, const QString &search_in,
                              const QString &subject, SynonymsManager::Match match_type,
                              Qt::CaseSensitivity case_sensitivity) const
{
    if (isSynonymString(subject)) {
        QString synonym_list_name = synonymStringToSynonym(subject);
        return compareWithSynonymName(oper, search_in, synonym_list_name, match_type,case_sensitivity);
    }
    else
        return StringCompareOperators::op(oper, search_in, subject, match_type, case_sensitivity);
}


/**
Executes the given operator for each synonym in the \a synonym_list_name against the string in \a search_in.
\see compare()
\param oper The test operation to use:  Op::Equals, Op::Contains, Op::StartsWith, Op::EndsWith, Op::RegexMatch, Op::AlwaysTrue, Op::AlwaysFalse, Op::NoOp.
\param search_in The string to search within.
\param synonym_list_name The synonym list to use. Ie. all synonyms in list are tried.
\param match_type Can be either Match::WholeWords or Match::SubString.
\param case_sensitivity Can be either Qt::CaseSensitive or Qt::CaseInsensitive.
\return True if a least one of the synonyms matches with the Operation used. */
bool SynonymsManager::compareWithSynonymName(SynonymsManager::Op oper, const QString &search_in,
                                             const QString &synonym_list_name, SynonymsManager::Match match_type,
                                             Qt::CaseSensitivity case_sensitivity) const
{
    auto it = find(synonym_list_name);
    if (it == end()) return false;
    bool match = false;

    for ( const QString &w : it->m_synonym_words) {
        match = StringCompareOperators::op(oper, search_in, w, match_type, case_sensitivity);
        if (match)
            break;
    }
    return match;
}


} //END namespace vdbclient
