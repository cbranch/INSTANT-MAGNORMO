#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

class ContactList;
class StatusWidget;
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
    StatusWidget *statusWidget;
    QDockWidget *statusWidgetDock;

public:
    IMThread *imThread;
    void connectThread();
};

#endif // MAINWINDOW_H
