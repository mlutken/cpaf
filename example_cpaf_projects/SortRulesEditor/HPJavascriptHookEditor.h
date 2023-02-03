#ifndef HPJAVASCRIPTHOOKEDITOR_H
#define HPJAVASCRIPTHOOKEDITOR_H

#include <QWidget>
#include <QTextOption>

class JavascriptEdit;
class QLabel;
class QCheckBox;
class QVBoxLayout;

class HPJavascriptHookEditor : public QWidget
{
    Q_OBJECT
public:
    explicit HPJavascriptHookEditor(QWidget *parent = nullptr);
    ~HPJavascriptHookEditor();

    void setHookHeader (const QString &hook_function_name, const QString &parameters);
    void setHookCodeBody (const QString &code_body);
    QString hookCodeBody () const;
    void setExamplesCode (const QString &examples_code);

    void setColorSchemeDefault();
    void setColorSchemeDark();

    QStringList keywords() const;
    void setKeywords(const QStringList &keywords);
    void setWordWrapMode ( QTextOption::WrapMode policy );

Q_SIGNALS:

public Q_SLOTS:

private Q_SLOTS:
    void showExamplesStateChangedSlot(int state);

private:
    void constructLayout();
    void contructConnections();
    QCheckBox *m_show_examples_chk;
    QLabel *m_hook_header;
    JavascriptEdit *m_hook_editor;
    QLabel *m_hook_footer;
    QLabel *m_examples_header;
    JavascriptEdit *m_examples_editor;
};

#endif // HPJAVASCRIPTHOOKEDITOR_H
