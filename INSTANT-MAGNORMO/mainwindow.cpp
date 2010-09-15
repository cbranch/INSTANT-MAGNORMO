#include "mainwindow.h"
#include <QDockWidget>
#include "contactlist.h"
#include "statuswidget.h"
#include "MAGNORMOBOT.h"
#include "Contact.h"
#include <QProgressBar>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setDockOptions(QMainWindow::AllowNestedDocks | QMainWindow::AllowTabbedDocks | QMainWindow::AnimatedDocks);
    contactListDock = new QDockWidget(tr("Contacts"), this);
    contactList = new ContactList();
    contactListDock->setWidget(contactList);
    addDockWidget(Qt::LeftDockWidgetArea, contactListDock);

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
    connect(bot, SIGNAL(contactDiscovered(QSharedPointer<Contact>)), contactList, SLOT(plantContact(QSharedPointer<Contact>)));
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
