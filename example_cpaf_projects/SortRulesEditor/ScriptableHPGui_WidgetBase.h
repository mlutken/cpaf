#ifndef SCRIPTABLEHPGUI_WIDGETBASE_H
#define SCRIPTABLEHPGUI_WIDGETBASE_H

#include <QWidget>
#include <vector>

class QStackedWidget;
class QComboBox;

class ScriptableHPGui_WidgetBase : public QWidget
{
    Q_OBJECT
public:
    explicit ScriptableHPGui_WidgetBase(QWidget *parent = nullptr);
    virtual ~ScriptableHPGui_WidgetBase() override;

    void addWidget (QWidget *widget, const QString &edit_mode_string_id, const QString &edit_mode_user_text);

Q_SIGNALS:

public Q_SLOTS:
    void setEditMode (const QString &edit_mode_string_id);

private Q_SLOTS:
    void currentComboIndexChangedSlot (int index);
private:
    using StrVec = std::vector<QString>;
    void constructLayout();
    void contructConnections();

    QComboBox *m_edit_mode_combo;
    QStackedWidget *m_widget_stack; //
    StrVec m_edit_modes;
};

#endif // SCRIPTABLEHPGUI_WIDGETBASE_H
