#ifndef IMAGELIBRARY_H
#define IMAGELIBRARY_H

#include <QMainWindow>
#include <QStringListModel>
#include <QListView>
#include <QToolBar>
#include <QImage>

#define THUMBNAIL_SIZE 128

class Item
{
    public:
        QString path;
        QImage thumbnail;
        Item(const QString &, const QImage &);
};

class Model: public QAbstractListModel
{
    Q_OBJECT

    private:
        QList<Item> items;

    public:
        Model();
        int rowCount(const QModelIndex &) const;
        QVariant data(const QModelIndex &, int) const;

    public slots:
        void addItem(const QString &, const QImage &);
};

class ImageLibrary : public QMainWindow
{
    Q_OBJECT

    private:
        Model model;
        QListView view;
        QToolBar toolbar;

    public:
        ImageLibrary(QWidget *parent = nullptr);
        ~ImageLibrary();
       void go();
       void addItem (const QString &, const QImage &);
};

class Worker: public QObject
{
    Q_OBJECT

    private:
        QString path;

    public:
        Worker (const QString &);
        static QImage Thumbnail(const QString &);
        static Item MappedItem(const QString &);

    public slots:
        void process();

    signals:
       void newItem(const QString &, const QImage &);
       void finished();
};

#endif // IMAGELIBRARY_H
