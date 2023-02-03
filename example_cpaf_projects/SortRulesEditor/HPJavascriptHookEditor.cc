#include "HPJavascriptHookEditor.h"
#include <iostream>
#include <QLabel>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <qtutility/JavascriptEdit.h>

using namespace std;

HPJavascriptHookEditor::HPJavascriptHookEditor(QWidget *parent)
    : QWidget(parent),
      m_show_examples_chk(new QCheckBox(tr("Show examples"),this)),
      m_hook_header(new QLabel(this)),
      m_hook_editor(new JavascriptEdit(this)),
      m_hook_footer(new QLabel("<b>}</b>", this)),
      m_examples_header(new QLabel("<b>" + tr("Examples") + "</b>", this)),
      m_examples_editor(new JavascriptEdit(this))
{
    m_hook_editor->setColorSchemeDark();
    m_examples_editor->setColorSchemeDefault();
    m_examples_editor->setReadOnly(true);
    m_examples_editor->setWordWrapMode(QTextOption::NoWrap);
    setWordWrapMode(QTextOption::NoWrap);

    constructLayout();
    contructConnections();
    m_show_examples_chk->setCheckState(Qt::Checked);
}

HPJavascriptHookEditor::~HPJavascriptHookEditor()
{

}

void HPJavascriptHookEditor::setHookHeader(const QString &hook_function_name, const QString &parameters)
{
    m_hook_header->setText("<b>" + hook_function_name + ": function(" + parameters + ")</b>");
}

void HPJavascriptHookEditor::setHookCodeBody(const QString &code_body)
{
    m_hook_editor->setPlainText(code_body);
}

QString HPJavascriptHookEditor::hookCodeBody() const
{
    return m_hook_editor->toPlainText();
}

void HPJavascriptHookEditor::setExamplesCode(const QString &examples_code)
{
    m_examples_editor->setPlainText(examples_code);
}

void HPJavascriptHookEditor::setColorSchemeDefault()
{
    m_hook_editor->setColorSchemeDefault();
}

void HPJavascriptHookEditor::setColorSchemeDark()
{
    m_hook_editor->setColorSchemeDark();
}

QStringList HPJavascriptHookEditor::keywords() const
{
    return m_hook_editor->keywords();
}

void HPJavascriptHookEditor::setKeywords(const QStringList &keywords)
{
    m_hook_editor->setKeywords(keywords);
}

void HPJavascriptHookEditor::setWordWrapMode(QTextOption::WrapMode policy)
{
    m_hook_editor->setWordWrapMode(policy);
}

void HPJavascriptHookEditor::showExamplesStateChangedSlot(int state)
{
    if (Qt::Checked == state) {
        m_examples_editor->show();
        m_examples_header->show();
    }
    else {
        m_examples_editor->hide();
        m_examples_header->hide();
    }
}

void HPJavascriptHookEditor::constructLayout()
{
    QVBoxLayout *top_vlayout(new QVBoxLayout());
    QHBoxLayout *topbar_hlayout(new QHBoxLayout());
    QHBoxLayout *editor_and_examples_hlayout(new QHBoxLayout());
    QVBoxLayout *editor_vlayout(new QVBoxLayout());
    QVBoxLayout *examples_vlayout(new QVBoxLayout());
    topbar_hlayout->addWidget(m_show_examples_chk);
    editor_vlayout->addWidget(m_hook_header);
    editor_vlayout->addWidget(m_hook_editor);
    editor_vlayout->addWidget(m_hook_footer);
    examples_vlayout->addWidget(m_examples_header);
    examples_vlayout->addWidget(m_examples_editor);
    examples_vlayout->addWidget(new QLabel((""), this)); // Just an empty "line" in layout
    editor_and_examples_hlayout->addLayout(editor_vlayout);
    editor_and_examples_hlayout->addLayout(examples_vlayout);
    top_vlayout->addLayout(topbar_hlayout);
    top_vlayout->addLayout(editor_and_examples_hlayout);
    setLayout(top_vlayout);
}


void HPJavascriptHookEditor::contructConnections()
{
    connect(m_show_examples_chk, SIGNAL(stateChanged(int)), this,SLOT(showExamplesStateChangedSlot(int)));
}

