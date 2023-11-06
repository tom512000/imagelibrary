#include "QApplication"
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QAction>

#include "imagelibrary.h"

ImageLibrary::ImageLibrary(QWidget *parent) : QMainWindow(parent), model{}, view{}, toolbar{}
{
    view.setModel(&model);
    toolbar.addAction ("GO !", this, &ImageLibrary::go);
    QMainWindow::addToolBar(&toolbar);
    QMainWindow::setCentralWidget(&view);
}

ImageLibrary::~ImageLibrary()
{
}

void  ImageLibrary::go()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                   QDir::homePath());
   if (dir.isNull())
       QMessageBox::warning(this, "WARNING", "Ya un prob...");

   else
       QMessageBox::information(this, "INFORMATION", "c good :)");
}
