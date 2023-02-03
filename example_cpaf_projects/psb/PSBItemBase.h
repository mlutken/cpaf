#ifndef PSBITEMBASE_H
#define PSBITEMBASE_H

#include <QWidget>
#include <vector>

class QLayout;

enum class PSBLayoutMode { none, vertical, horizontal, fullScreen };

class PSBItemBase : public QWidget
{
    Q_OBJECT
public:
    static PSBItemBase *create_DBG(QWidget *parent, const QString &widget_headline, int child_count = 0);
    explicit PSBItemBase(QWidget *parent = 0);

    QWidget* widgetHeadline() const;
    void setHeadline(QWidget* widget_headline);
    void setHeadline(const QString &widget_headline);
    QString headlineString() const;
    void appendChildWidget(QWidget *child);

    void setHeadline_DBG(const QString &widget_headline, bool add_number = true);

    PSBLayoutMode psbLayoutMode() const;
    void setPsbLayoutMode(const PSBLayoutMode &psb_layout_mode);

    static void removeLayout(QWidget* widget);
    void defaultTestingConstructor();

signals:

public slots:

private:
    virtual void doSetPsbLayoutMode(const PSBLayoutMode &psb_layout_mode);


    PSBLayoutMode m_psb_layout_mode = PSBLayoutMode::none;
    QWidget *m_widget_headline;
    QWidget *m_child_container_widget;
    std::vector<QWidget *> m_child_widgets;
    QLayout *m_toplevel_layout;
    QLayout *m_children_layout;


    int setHeadline_DBG_counter = 0;

};

#endif // PSBITEMBASE_H
