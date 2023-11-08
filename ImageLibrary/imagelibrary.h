#ifndef IMAGELIBRARY_H
#define IMAGELIBRARY_H

#include <QMainWindow>
#include <QStringListModel>
#include <QListView>
#include <QToolBar>

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
       void  go();
};

class Worker: public QObject
{
    private:
        QString path;
        Worker (const QString &);
        void newItem();

    public:
        void process();

};

#endif // IMAGELIBRARY_H
