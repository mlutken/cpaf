#include "PSB.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

PSB::PSB(QWidget *parent) : QWidget(parent)
{
    auto label = new QLabel("PSB");
    auto layout = new QHBoxLayout();
    layout->addWidget(label);

    setLayout(layout);
}
