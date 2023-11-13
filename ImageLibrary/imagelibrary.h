#ifndef IMAGELIBRARY_H
#define IMAGELIBRARY_H

#include <QMainWindow>
#include <QStringListModel>
#include <QListView>
#include <QToolBar>
#include <QImage>

class ImageLibrary : public QMainWindow
{
    Q_OBJECT

    private:
        QStringListModel model;
        QListView view;
        QToolBar toolbar;

    public:
        ImageLibrary(QWidget *parent = nullptr);
        ~ImageLibrary();
       void go();
       void addItem (const QString &);
};

class Worker: public QObject
{
    Q_OBJECT

    private:
        QString path;

    public:
        Worker (const QString &);
    public slots:
        void process();

    signals:
       void newItem(const QString &);
       void finished();
};

class Item
{
    public:
        QString path;
        QImage thumbnail;
        Item(const QString &, const QImage &);
};

class Model: public QAbstractListModel
{
    private:
        QList<Item> items;

    public:
        Model(const QList<Item> &);
        int rowCount(const QModelIndex &) const;
        QVariant data(const QModelIndex &, int) const;

    public slots;
        void addItem(QString, QImage);
};

#endif // IMAGELIBRARY_H
