#include "QApplication"
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QAction>
#include <QSettings>
#include <QStringList>
#include <QDir>

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
    QSettings settings;
    QString init = settings.value("dir", QDir::homePath()).toString();
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), init);

   if (dir.isNull())
       QMessageBox::warning(this, "WARNING", "Ya un prob...");
   else {
       QMessageBox::information(this, "INFORMATION", "c good :)");
       settings.setValue("dir", dir);
       Worker worker(dir);
       connect(&worker, &Worker::newItem, this, &ImageLibrary::addItem);
       worker.process();
   }

}

Worker::Worker(const QString & path): path(path)
{
}

void Worker::process()
{
    QStringList queue;
    queue << path;

    while (!queue.isEmpty()) {
        QString currentPath = queue.takeFirst();
        QDir currentDir(currentPath);
        QFileInfoList entries = currentDir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);

        foreach (const QFileInfo &entry, entries) {
            if (entry.isDir()) {
                queue << entry.filePath();
            } else if (entry.isFile() && (entry.suffix() == "png" || entry.suffix() == "jpg" || entry.suffix() == "jpeg")) {
                emit newItem(entry.filePath());
            }
        }
    }
}

void ImageLibrary::addItem (const QString & item)
{
  QStringList list = model.stringList ();
  model.setStringList (list << item);
}
