#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

class ContactList;
class QDockWidget;
class IMThread;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    ContactList *contactList;
    QDockWidget *contactListDock;

public:
    IMThread *imThread;
    void connectThread();
};

#endif // MAINWINDOW_H
