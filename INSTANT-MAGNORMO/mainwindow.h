#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QHash>
#include <QToolBar>
#include <QAction>
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
    QToolBar *toolBar;

    void createToolBar();
    void connectAccount(QString username, QString password, QString server, int port);

public:
    MAGNORMOBOT *bot;
    void startAccountManager(bool specificRequest);

public slots:
    void connected();
    void disconnected();

    void startConversation(QString jid);
    void handleConversationDestroyed(QObject* list);

private slots:
    void getAccountManager();
    void getContactWindow();
};

#endif // MAINWINDOW_H
