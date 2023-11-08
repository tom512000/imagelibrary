#include "imagelibrary.h"

#include <QApplication>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImageLibrary w;
    w.show();
    QCoreApplication::setOrganizationName("siko0001");
    QCoreApplication::setApplicationName("ImageLibrary");
    return a.exec();
}
