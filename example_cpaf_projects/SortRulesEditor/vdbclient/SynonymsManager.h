#ifndef VDBCLIENT_SYNONYMSMANAGER_H
#define VDBCLIENT_SYNONYMSMANAGER_H

#include <vector>
#include <QString>

#include <evbase/SerializationUtils.h>
#include <evbase/StringCompareOperators.h>



namespace vdbclient {

class Preferences;

class SynonymsManager
{
public:
    using Op = evbase::StringCompareOperators::Op;
    using Match = evbase::StringCompareOperators::Match;

    // --- Nested definitions
    using SynonymWordsVec = std::vector<QString>;
    struct Synonym
    {
        Synonym() = default;
        Synonym(const Synonym &) = default;
        Synonym& operator=(const Synonym &) = default;
        explicit Synonym(const QString &name)
            : m_synonym_list_name(name)
        {}

        Synonym(const QString &name, const SynonymWordsVec& words)
            : m_synonym_list_name(name), m_synonym_words(words)
        {}

        Synonym(const QString &name, SynonymWordsVec&& words)
            : m_synonym_list_name(name), m_synonym_words(std::move(words))
        {}

        const SynonymWordsVec &synonymWords() const { return m_synonym_words; }
        QString synonymWordsAsString() const { return evbase::containerToQString(m_synonym_words, ',', false);}
        friend bool operator<(const Synonym & lhs, const Synonym& rhs)
        {
            return lhs.m_synonym_list_name.toLower() < rhs.m_synonym_list_name.toLower();
        }

        friend bool operator==(const Synonym & lhs, const Synonym& rhs)
        {
            return lhs.m_synonym_list_name == rhs.m_synonym_list_name;
        }


        QString m_synonym_list_name;
        SynonymWordsVec m_synonym_words;
    };

    using SynonymsVec = std::vector<Synonym>;
    using iterator = SynonymsVec::iterator;
    using const_iterator = SynonymsVec::const_iterator;

    // --- SynonymsManager class main definition ---
    SynonymsManager();
    const static SynonymWordsVec emptySynonymWordsVec;
    static SynonymsManager& instance();
    static bool isSynonymString (const QString &string_id);
    static QString synonymStringToSynonym (const QString &string_id);

    iterator begin() {return m_synonyms.begin(); }
    const_iterator begin() const {return m_synonyms.begin(); }
    iterator end() { return m_synonyms.end(); }
    const_iterator end() const { return m_synonyms.end(); }
    size_t size() const { return m_synonyms.size(); }
    void clear() { m_synonyms.clear(); }
    Synonym& operator[](size_t i);
    const Synonym& operator[](size_t i) const;
    Synonym* at(size_t i);
    const Synonym* at(size_t i) const;
    iterator find(const QString &synonym_list_name);
    const_iterator find(const QString &synonym_list_name) const;
    const SynonymsVec &synonyms() const { return m_synonyms; }

    void storeInPreferences (Preferences *preferences, QString path);
    void loadFromPreferences (Preferences *preferences, QString path);

    void insert(iterator pos, size_t count, const Synonym& synonym);
    iterator erase( iterator it );
    iterator erase( iterator first, iterator last );
    void addSynonymList (const QString &synonym_list_name, const QString &synonyms_comma_separated);
    void removeSynonymList (const QString &synonym_list_name);
    const SynonymWordsVec &synonymsForName (const QString &synonym_list_name) const;
//    Synonym searchSynonym (const QString &synonym) const;
    void sortSynonyms() ;

    bool compare(Op oper, const QString &search_in, const QString &subject,
                 Match match_type, Qt::CaseSensitivity case_sensitivity) const;

    bool compareWithSynonymName(Op oper, const QString &search_in, const QString &synonym_list_name,
                                Match match_type, Qt::CaseSensitivity case_sensitivity) const;
private:
    static const QString m_default_preferences_path;
    SynonymsVec m_synonyms;
};

} //END namespace vdbclient

#endif // VDBCLIENT_SYNONYMSMANAGER_H
