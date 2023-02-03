#include "PatientBrowserStudyHeaderView.h"

#include <boost/foreach.hpp>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>

PatientBrowserStudyHeaderView::PatientBrowserStudyHeaderView(QWidget *parent) :
    QWidget(parent), m_compact_mode(true)
{
    // --- Set default positions of labels ---
//    m_label_name_to_pos["study_date"] = 0;
//    m_label_name_to_pos["study_title"] = 1;
//    m_label_name_to_pos["study_ascension_number"] = 2;

    m_pos_to_label_name.push_back("study_date");
    m_pos_to_label_name.push_back("study_title");
    m_pos_to_label_name.push_back("study_ascension_number");

    m_label_widgets.push_back(new QLabel("0", this)); // 0
    m_label_widgets.push_back(new QLabel("1", this)); // 1
    m_label_widgets.push_back(new QLabel("2", this)); // 2

    m_layout = new QGridLayout;
    m_layout->addWidget(m_label_widgets[0], 0, 0);
    m_layout->addWidget(m_label_widgets[1], 0, 1);
    m_layout->addWidget(m_label_widgets[2], 1, 0, 1, 2);

    //m_layout->setColumnStretch(0, 1);
    setLayout(m_layout);

    updateView();
}


void PatientBrowserStudyHeaderView::refreshView()
{

}

void PatientBrowserStudyHeaderView::setViewMode(int /*view_mode_mask*/)
{

}


void PatientBrowserStudyHeaderView::updateView()
{
    const unsigned int N = m_label_widgets.size();
    for ( unsigned int i = 0; i < N; ++i) {
        QLabel* const label = m_label_widgets[i];
        label->setText( getLabelText(labelPosToName(i)));
    }
    if (!m_compact_mode) m_label_widgets[2]->hide();
}

QString PatientBrowserStudyHeaderView::getLabelText(const QString &label_name)
{
    if (label_name == "study_date") return "22/5";
    else if (label_name == "study_title") return "Long long study title for testing";
    return label_name;
}

QString PatientBrowserStudyHeaderView::labelPosToName(unsigned int pos)
{
    return m_pos_to_label_name[pos];
}

//unsigned int PatientBrowserStudyHeaderView::labelNameToPosition(const QString &label_name)
//{
//    return m_label_name_to_pos[label_name];
//}

