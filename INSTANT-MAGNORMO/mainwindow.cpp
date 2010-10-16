#include "mainwindow.h"
#include <QDockWidget>
#include "contactlist.h"
#include "contactmodel.h"
#include "statuswidget.h"
#include "conversationwidget.h"
#include "MAGNORMOBOT.h"
#include "Contact.h"
#include <QProgressBar>
#include <QLabel>
#include <QTreeWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    bot(0)
{
    setDockOptions(QMainWindow::AllowNestedDocks | QMainWindow::AllowTabbedDocks | QMainWindow::AnimatedDocks);

    // Contact List
    contactListDock = new QDockWidget(tr("Contacts"), this);
    contactList = new ContactList();
    contactListDock->setWidget(contactList);
    addDockWidget(Qt::LeftDockWidgetArea, contactListDock);
    connect(contactList, SIGNAL(conversationInitiated(QString)), SLOT(startConversation(QString)));

    // Contact Model
    contactModel = new ContactModel(contactList);
    contactList->setModel(contactModel);

    // Connection Manager
    connectionManager = new ConnectionManger(this,contactModel);

    // Status widget thinggggyyiiieiiiidiiweieiii
    statusWidgetDock = new QDockWidget(tr("Status"), this);
    statusWidget = new StatusWidget();
    statusWidgetDock->setWidget(statusWidget);
    addDockWidget(Qt::LeftDockWidgetArea, statusWidgetDock);

    // Account Manager Dialog
    accMgrDiag = new accountmanagerdialog();

    // Menu Bar
    toolBar = new QToolBar(this);
    createToolBar();
    this->addToolBar(toolBar);
}

MainWindow::~MainWindow()
{
}

void MainWindow::connectAccount(QString username, QString password, QString server, int port)
{
	/*
    contactList->setModel(contacts);
    connect(bot, SIGNAL(connected()), SLOT(connected()));
    connect(bot, SIGNAL(disconnected()), SLOT(disconnected()));
    connect(bot, SIGNAL(openConversationWindow(QString)), SLOT(startConversation(QString)), Qt::BlockingQueuedConnection);
    statusWidget->label->setText(tr("Connecting"));
    statusWidget->progress->setMinimum(0);
    statusWidget->progress->setMaximum(0);
    statusWidget->progress->setValue(0);
    statusWidgetDock->setVisible(true);

    bot->start();*/
}

void MainWindow::connected()
{
    statusWidgetDock->setVisible(false);
}

void MainWindow::disconnected()
{
    statusWidget->label->setText(tr("Disconnected"));
    statusWidgetDock->setVisible(true);
}

void MainWindow::startConversation(QString jid)
{
    MAGNORMOBOT *bot = sender();
    if(jid!="GROUP") {
        //QString contactName("Conversation");
        QString contactName = bot->getNameFromJid(jid);
        ConversationDict::const_iterator iter = conversationDict.find(jid);
        if (iter != conversationDict.end()) {
            // Select already existing conversation
            iter.value()->setVisible(true);
            statusWidgetDock->setFocus(Qt::OtherFocusReason);
        } else {
            // Open new conversation
            ConversationWidget *convo = new ConversationWidget(jid);
            QDockWidget *dock = new QDockWidget(contactName);
            dock->setWidget(convo);
            // Is there an existing conversation?
            if (conversationDict.empty()) {
                addDockWidget(Qt::LeftDockWidgetArea, dock);
            } else {
                tabifyDockWidget(conversationDict.begin().value(), dock);
            }
            conversationDict.insert(jid, dock);
            connect(convo, SIGNAL(destroyed(QObject*)), SLOT(handleConversationDestroyed(QObject*)));
            connect(convo, SIGNAL(sendMessage(QString,QString)), bot, SLOT(sendMessage(QString,QString)));
            connect(bot, SIGNAL(spewMessage(QString,QString)), convo, SLOT(messageRevieved(QString,QString)));
            connect(dock, SIGNAL(visibilityChanged(bool)),convo,SLOT(onVisibleChange(bool)));
            connect(convo, SIGNAL(dissapearWindow(QString)),bot,SLOT(dissapearedWindow(QString)));
        }
    }
}

void MainWindow::handleConversationDestroyed(QObject *list)
{
    conversationDict.remove(qobject_cast<ConversationWidget*>(list)->jid);
}

void MainWindow::startAccountManager(bool specificRequest)
{
    if(accMgrDiag->needed||specificRequest) {
        switch(accMgrDiag->exec()) {
            case QDialog::Accepted:
            {
                QList<Account*> activeAccounts = accMgrDiag->getActiveAccounts();
                // For now we will select the first active one as we dont have multiple account support yet
                if(activeAccounts.size()==0) {
                    printf("You suck balls\n");
                    return;
                }
                // Add active accounts loop here
                Account* acc = activeAccounts.first();

                connectAccount(acc->user,acc->password,acc->server,acc->port);
                return;
            }
            case QDialog::Rejected:
            {
                return;
            }
        }
    } else {
        QList<Account*> activeAccounts = accMgrDiag->getActiveAccounts();
        // For now we will select the first active one as we dont have multiple account support yet
        Account* acc = activeAccounts.first();

        connectAccount(acc->user,acc->password,acc->server,acc->port);
        return;
    }
}

void MainWindow::createToolBar()
{
    // Accounts menu
    QAction *accountManagerAction = toolBar->addAction(QIcon(":/icons/accounts"),"THE ACCOUNT MANGER");
    connect(accountManagerAction,SIGNAL(triggered()),this,SLOT(getAccountManager()));

    // Window menu
    QAction *contactWindowAction = toolBar->addAction(QIcon(":/icons/contacts"),"YOUR SUPPOSED \"MATES\"");
    connect(contactWindowAction,SIGNAL(triggered()),this,SLOT(getContactWindow()));
}

void MainWindow::getAccountManager()
{
    startAccountManager(true);
}

void MainWindow::getContactWindow()
{
    contactListDock->setVisible(true);
}
