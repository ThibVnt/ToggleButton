#include "togglebutton.h"

#include <QApplication>
#include <QVBoxLayout>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget *widget = new QWidget;
    QVBoxLayout layout;
    widget->setLayout(&layout);

    auto *w = new ToggleButton("ON","OFF");
    layout.addWidget(w);

    widget->show();
    return a.exec();
}
