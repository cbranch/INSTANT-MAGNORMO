#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QHash>
#include "accountmanagerdialog.h"

class ContactList;
class StatusWidget;
class QDockWidget;
class MAGNORMOBOT;
class ConversationWidget;

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

    accountmanagerdialog *accMgrDiag;

public:
    MAGNORMOBOT *bot;
    void connectAccount(QString username, QString password, QString server, int port);
    void startAccountManager(bool specificRequest);

public slots:
    void connected();
    void disconnected();

    void startConversation(QString jid);
    void handleConversationDestroyed(QObject* list);
};

#endif // MAINWINDOW_H
