#include "imagelibrary.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImageLibrary w;
    w.show();
    w.go();
    return a.exec();
}
