#include "PSBPatient.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

PSBPatient::PSBPatient(QWidget *parent) : QWidget(parent)
{
    static int i = 0;
    i++;
    auto label = new QLabel("PSBPatient-" + i);
    auto layout = new QHBoxLayout();
    layout->addWidget(label);

    setLayout(layout);

}
