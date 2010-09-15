#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

class ContactList;
class StatusWidget;
class QDockWidget;
class MAGNORMOBOT;

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
    MAGNORMOBOT *bot;
    void connectAccount(QString username, QString password, QString server, int port);

public slots:
    void connected();
    void disconnected();
};

#endif // MAINWINDOW_H
