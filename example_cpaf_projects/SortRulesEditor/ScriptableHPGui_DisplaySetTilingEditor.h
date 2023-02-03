#ifndef SCRIPTABLEHPGUI_DISPLAYSETTILINGEDITOR_H
#define SCRIPTABLEHPGUI_DISPLAYSETTILINGEDITOR_H

#include <QWidget>

class QSpinBox;

class ScriptableHPGui_DisplaySetTilingEditor : public QWidget
{
public:
    explicit ScriptableHPGui_DisplaySetTilingEditor(QWidget *parent = 0);
    virtual ~ScriptableHPGui_DisplaySetTilingEditor() override;
Q_SIGNALS:

public Q_SLOTS:

private Q_SLOTS:

private:
    void constructLayout();
    void contructConnections();

    QSpinBox *m_rows_spinbox;
    QSpinBox *m_columns_spinbox;
};

#endif // SCRIPTABLEHPGUI_DISPLAYSETTILINGEDITOR_H
