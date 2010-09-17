#include "mainwindow.h"
#include <QDockWidget>
#include "contactlist.h"
#include "statuswidget.h"
#include "conversationwidget.h"
#include "MAGNORMOBOT.h"
#include "Contact.h"
#include <QProgressBar>
#include <QLabel>
#include <QTreeWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setDockOptions(QMainWindow::AllowNestedDocks | QMainWindow::AllowTabbedDocks | QMainWindow::AnimatedDocks);
    contactListDock = new QDockWidget(tr("Contacts"), this);
    contactList = new ContactList();
    contactListDock->setWidget(contactList);
    addDockWidget(Qt::LeftDockWidgetArea, contactListDock);
    connect(contactList, SIGNAL(conversationInitiated(QString)), SLOT(startConversation(QString)));

    statusWidgetDock = new QDockWidget(tr("Status"), this);
    statusWidget = new StatusWidget();
    statusWidgetDock->setWidget(statusWidget);
    addDockWidget(Qt::LeftDockWidgetArea, statusWidgetDock);
}

MainWindow::~MainWindow()
{
}

void MainWindow::connectAccount(QString username, QString password, QString server, int port)
{
    bot = new MAGNORMOBOT(username, password, server, port);
    connect(bot, SIGNAL(connected()), SLOT(connected()));
    connect(bot, SIGNAL(disconnected()), SLOT(disconnected()));
    connect(bot, SIGNAL(contactPresenceUpdate(QSharedPointer<Contact>)), contactList, SLOT(plantContact(QSharedPointer<Contact>)));
    statusWidget->label->setText(tr("Connecting"));
    statusWidget->progress->setMinimum(0);
    statusWidget->progress->setMaximum(0);
    statusWidget->progress->setValue(0);
    statusWidgetDock->setVisible(true);
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
    if(jid!="GROUP") {
        QString contactName("Conversation");
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
            connect(bot, SIGNAL(spewMessage(QString,QString)), convo, SLOT(messageRevieved(QString,QString)));
        }
    }
}

void MainWindow::handleConversationDestroyed(QObject *list)
{
    conversationDict.remove(qobject_cast<ConversationWidget*>(list)->jid);
}
