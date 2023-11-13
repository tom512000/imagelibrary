#include "QApplication"
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QAction>
#include <QSettings>
#include <QStringList>
#include <QDir>
#include <QThread>
#include <QtConcurrent>

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

void ImageLibrary::go()
{
    QSettings settings;
    QString lastDir = settings.value("lastDirectory", QDir::homePath()).toString();

    QString selectedDir = QFileDialog::getExistingDirectory(this, "Select Directory", lastDir);

    if (!selectedDir.isEmpty()) {
        settings.setValue("lastDirectory", selectedDir);
        QtConcurrent::run([=](){
            Worker worker(selectedDir);
            connect(&worker, &Worker::newItem, this, &ImageLibrary::addItem);
            worker.process();

        });
        /*
        Worker *worker = new Worker(selectedDir);
        connect(worker, &WQAbstractListModelorker::newItem, this, &ImageLibrary::addItem);
        QThread *thread = new QThread;
        worker->moveToThread(thread);
        connect(thread, &QThread::started, worker, &Worker::process);
        connect(worker, &Worker::finished, thread, &QThread::quit);
        connect(worker, &Worker::finished, worker, &Worker::deleteLater);
        connect(thread, &QThread::finished, thread, &QThread::deleteLater);
        thread->start();
        */
    } else {
        QMessageBox::warning(this, "Warning", "Operation canceled or no directory selected!");
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

Item::Item(const QString & path, const QImage & thumbnail): path(path), thumbnail(thumbnail)
{
}

Model::Model(const QList<Item> & items): items(items)
{
}

int Model::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return items.count();
}

QVariant Model::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::ToolTipRole) {
        const Item &item = items.at(index.row());
        if (role == Qt::DisplayRole)
            return item.path;
        else if (role == Qt::ToolTipRole)
            return QVariant(item.path);
    }

    return QVariant();
}

void addItem(QString path, QImage thumbnail)
{

}
