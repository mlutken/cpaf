#include "PSBItemBase.h"

#include <iostream>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

using namespace std;

PSBItemBase *PSBItemBase::create_DBG(QWidget *parent, const QString& widget_headline, int child_count)
{
    auto item = new PSBItemBase(parent);
    item->setHeadline_DBG(widget_headline);


    for ( int n = 1 ; n <= child_count; n++) {
        auto child = new PSBItemBase(item);
        child->setHeadline_DBG(item->headlineString() + " - child " + QString::number(n), false);
//        child->setHeadline_DBG("C");
        item->appendChildWidget(child);
    }

    return item;
}

void PSBItemBase::defaultTestingConstructor()
{
    static int i = 0;
    i++;
    auto label = new QLabel("PSBItem X" + QString::number(i));
    label->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    label->setFrameShape(QFrame::Box);
    label->setFrameShadow(QFrame::Raised);
    label->setAlignment(Qt::AlignHCenter);

    m_widget_headline = label;


    auto layout = new QVBoxLayout();
    layout->addWidget(m_widget_headline);
    setLayout(layout);
}

PSBItemBase::PSBItemBase(QWidget *parent) : QWidget(parent)
{
    m_child_container_widget = new QWidget(this);

//    defaultTestingConstructor();
//    setHeadline("PSBItem Y");

}

QWidget *PSBItemBase::widgetHeadline() const
{
    return m_widget_headline;
}

void PSBItemBase::setHeadline(QWidget* widget_headline)
{
    m_widget_headline = widget_headline;
}

void PSBItemBase::setHeadline(const QString& widget_headline)
{
    PSBItemBase::removeLayout(this);
    auto label = new QLabel(widget_headline, this);
    label->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    label->setFrameShape(QFrame::Box);
    label->setFrameShadow(QFrame::Raised);
    label->setAlignment(Qt::AlignHCenter);
    m_widget_headline = label;
    auto layout = new QVBoxLayout();
    layout->addWidget(m_widget_headline);

    setLayout(layout);

}

QString PSBItemBase::headlineString() const
{
    if (auto label = dynamic_cast<QLabel*>(m_widget_headline))
        return label->text();

    return "";
}

void PSBItemBase::appendChildWidget(QWidget *child)
{
    m_child_widgets.push_back(child);
}

/** Debug function to set headline auto-incrementing a number so we can tell them apart. */
void PSBItemBase::setHeadline_DBG(const QString& widget_headline, bool add_number)
{
    setHeadline_DBG_counter++;
    if (add_number)
        m_widget_headline = new QLabel(widget_headline + "-" + QString::number(setHeadline_DBG_counter));
    else
        m_widget_headline = new QLabel(widget_headline);
}

PSBLayoutMode PSBItemBase::psbLayoutMode() const
{
    return m_psb_layout_mode;
}

void PSBItemBase::setPsbLayoutMode(const PSBLayoutMode &psb_layout_mode)
{
    m_psb_layout_mode = psb_layout_mode;
    PSBItemBase::removeLayout(this);
    PSBItemBase::removeLayout(m_child_container_widget);
    doSetPsbLayoutMode(psb_layout_mode);
}

void PSBItemBase::removeLayout(QWidget *widget)
{
    if (!widget)
        return;
    QLayout* layout = widget->layout();
    if (layout != 0)
    {
        QLayoutItem *item;
        while ((item = layout->takeAt(0)) != 0)
            layout->removeItem (item);
        delete layout;
    }

}

void PSBItemBase::doSetPsbLayoutMode(const PSBLayoutMode &psb_layout_mode)
{
    // TODO: Remove old layouts!!
    switch (psb_layout_mode) {
    case PSBLayoutMode::vertical:
        m_toplevel_layout = new QVBoxLayout();
        m_children_layout = new QVBoxLayout();
        break;
    case PSBLayoutMode::horizontal:
        m_toplevel_layout = new QVBoxLayout();
        m_children_layout = new QHBoxLayout();
        break;
    default:
        break;
    }
//    if (psb_layout_mode == PSBLayoutMode::vertical) {
//        m_toplevel_layout = new QVBoxLayout();
//        m_children_layout = new QVBoxLayout();
//    }

    if (auto label = dynamic_cast<QLabel*>(m_widget_headline)) {
        label->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        label->setFrameShape(QFrame::Box);
        label->setFrameShadow(QFrame::Raised);
        label->setAlignment(Qt::AlignHCenter);
//        cout << "SSSSSSSSSSSS" << endl;
    }
    m_toplevel_layout->addWidget(m_widget_headline);
    for ( auto child : m_child_widgets) {
        if (auto psb_item_base = dynamic_cast<PSBItemBase*>(child)) {
            psb_item_base->setPsbLayoutMode(psb_layout_mode);
//            label->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
//            label->setFrameShape(QFrame::Box);
//            label->setFrameShadow(QFrame::Raised);
//            label->setAlignment(Qt::AlignHCenter);
//            cout << "CCCCCCCCCCC" << endl;
        }
        m_children_layout->addWidget(child);
    }
    m_child_container_widget->setLayout(m_children_layout);
    m_toplevel_layout->addWidget(m_child_container_widget);

    setLayout(m_toplevel_layout);
}
