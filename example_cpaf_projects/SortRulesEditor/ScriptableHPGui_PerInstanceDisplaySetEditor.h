#ifndef SCRIPTABLEHPGUI_PERINSTANCEDISPLAYSETEDITOR_H
#define SCRIPTABLEHPGUI_PERINSTANCEDISPLAYSETEDITOR_H

#include <QWidget>

class QSpinBox;
class QLineEdit;

/** Simple GUI to specify per instance display sets.*/
class ScriptableHPGui_PerInstanceDisplaySetEditor : public QWidget
{
    Q_OBJECT
public:
    explicit ScriptableHPGui_PerInstanceDisplaySetEditor(QWidget *parent = 0);
    virtual ~ScriptableHPGui_PerInstanceDisplaySetEditor() override;

Q_SIGNALS:

public Q_SLOTS:

private Q_SLOTS:
private:
    void constructLayout();
    void contructConnections();

    QLineEdit *m_modalities_list;
    QSpinBox *m_per_instance_threshold_spinbox;
};

#endif // SCRIPTABLEHPGUI_PERINSTANCEDISPLAYSETEDITOR_H
