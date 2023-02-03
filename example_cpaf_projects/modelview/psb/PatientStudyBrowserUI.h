#ifndef PATIENTSTUDYBROWSERUI_H
#define PATIENTSTUDYBROWSERUI_H

#include <QWidget>


class PatientStudyBrowserUI : public QWidget
{
    Q_OBJECT
public:
    explicit PatientStudyBrowserUI(QWidget *parent = 0);


signals:

public slots:
    void dbgSlot1();
    void dbgSlot2();
    void dbgSlot3();
    void dbgSlot4();
    void dbgSlot5();

private:
    void dbgCreateContent1();
};

#endif // PATIENTSTUDYBROWSERUI_H
