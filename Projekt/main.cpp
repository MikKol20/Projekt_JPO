#include "GUIApp.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GUIApp w;
    w.show();
    return a.exec();
}
