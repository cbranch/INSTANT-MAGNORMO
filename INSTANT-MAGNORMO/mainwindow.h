#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QHash>

class ContactList;
class StatusWidget;
class QDockWidget;
class MAGNORMOBOT;

typedef QHash<QString, QDockWidget*> ConversationDict;

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
    ConversationDict conversationDict;

public:
    MAGNORMOBOT *bot;
    void connectAccount(QString username, QString password, QString server, int port);

public slots:
    void connected();
    void disconnected();

    void startConversation(QString jid);
};

#endif // MAINWINDOW_H
