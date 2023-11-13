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

#endif // IMAGELIBRARY_H
