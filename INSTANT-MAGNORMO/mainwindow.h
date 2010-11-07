#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QApplication>
#include <QHash>
#include <QToolBar>
#include <QFile>
#include <QAction>
#include <QDockWidget>
#include <QProgressBar>
#include <QLabel>
#include <QTreeWidget>

#include "accountmanagerdialog.h"
#include "contactlist.h"
#include "contactmodel.h"
#include "statuswidget.h"
#include "conversationwidget.h"
#include "MAGNORMOBOT.h"
#include "Contact.h"
#include "connectionmanger.h"

class ContactList;
class StatusWidget;
class QDockWidget;
class MAGNORMOBOT;
class ConversationWidget;
class ConnectionManger;
class ContactModel;
#include "Contact.h"

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
	ConnectionManger *connectionManager;
	ContactModel *contactModel;

    void createToolBar();
    void connectAccount(QString username, QString password, QString server, int port);
    void setTheme(QString themeLocation);

    void readSettings();
    void writeSettings();

	void closeEvent(QCloseEvent *event);

public:
    void startAccountManager(bool specificRequest);

public slots:
    void connected();
    void disconnected();

    void startConversation(Contact);
    void handleConversationDestroyed(QObject* list);

private slots:
    void getAccountManager();
    void getContactWindow();
};

#endif // MAINWINDOW_H
