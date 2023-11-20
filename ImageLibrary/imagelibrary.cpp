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

ImageLibrary::ImageLibrary(QWidget *parent): QMainWindow(parent), model{}, view{}, toolbar{}
{
    view.setModel(&model);
    view.setViewMode(QListView::IconMode);
    view.setIconSize(QSize(THUMBNAIL_SIZE, THUMBNAIL_SIZE));
    view.setGridSize(QSize(THUMBNAIL_SIZE + 10, THUMBNAIL_SIZE + 10));
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
    QString lastDir = settings.value("Dernier dossier", QDir::homePath()).toString();

    QString selectedDir = QFileDialog::getExistingDirectory(this, "Sélectionner dossier", lastDir);

    if (!selectedDir.isEmpty()) {
        settings.setValue("Dernier dossier", selectedDir);
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
        QMessageBox::warning(this, "Attention", "Action annulée");
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
                queue << entry.absoluteFilePath();
            } else if (entry.isFile() && (entry.suffix() == "png" || entry.suffix() == "jpg" || entry.suffix() == "jpeg")) {
                emit newItem(entry.absoluteFilePath(), Worker::Thumbnail(entry.absoluteFilePath()));
            }
        }
    }
}

QImage Worker::Thumbnail(const QString & filePath)
{
    QImage originalImage(filePath);

        if (originalImage.isNull()) {
            qDebug() << "ça marche pas" << filePath;
            return QImage();
        }

        QSize newSize(THUMBNAIL_SIZE, THUMBNAIL_SIZE);
        QImage thumbnail = originalImage.scaled(newSize, Qt::KeepAspectRatio);

        return thumbnail;
}

void ImageLibrary::addItem (const QString & path,const QImage & thumbnail)
{
  model.addItem(path, thumbnail);
}

Item::Item(const QString & path, const QImage & thumbnail): path(path), thumbnail(thumbnail)
{
}

Model::Model(): items()
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
    const Item &item = items.at(index.row());
    switch (role) {
        case Qt::DisplayRole:
            return item.path;
        case Qt::ToolTipRole:
            return QVariant(item.path);
        case Qt::DecorationRole:
            return QVariant::fromValue(item.thumbnail);
        }

    return QVariant();
}

void Model::addItem(const QString &path, const QImage &thumbnail)
{
    int row = rowCount(QModelIndex());
    beginInsertRows(QModelIndex(), row, row);
    items.append(Item(path,thumbnail));
    endInsertRows();
}
