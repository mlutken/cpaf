#ifndef PATIENTBROWSERBASEITEM_H
#define PATIENTBROWSERBASEITEM_H

#include <vector>
#include <QString>

class PatientBrowserBaseItem
{
public:
    typedef std::vector<PatientBrowserBaseItem*> BaseItemVec;

    PatientBrowserBaseItem();
    explicit PatientBrowserBaseItem(PatientBrowserBaseItem *parent);
    virtual ~PatientBrowserBaseItem();

    void setParent (PatientBrowserBaseItem *parent);
    void appendChild (PatientBrowserBaseItem *child);

    // --- Parent/child functions ---
    bool hasChildren () const { return !m_children.empty(); }
    const BaseItemVec& children () const { return m_children; }

    BaseItemVec::const_iterator begin () const { return m_children.begin(); }
    BaseItemVec::const_iterator end () const { return m_children.end(); }
    BaseItemVec::iterator begin () { return m_children.begin(); }
    BaseItemVec::iterator end () { return m_children.end(); }

    QString displayText() const { return doDisplayText(); }

    QString dbgText() const;
    void setDbgText(const QString& text) { m_dbg_text = text; }

private:
    virtual QString doDisplayText () const = 0;

    PatientBrowserBaseItem* m_parent;
    BaseItemVec m_children;
    QString m_dbg_text;
};

#endif // PATIENTBROWSERBASEITEM_H
