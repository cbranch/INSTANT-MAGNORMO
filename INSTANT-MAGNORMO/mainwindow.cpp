#include "mainwindow.h"
#include <QDockWidget>
#include "contactlist.h"
#include "statuswidget.h"
#include "MAGNORMOBOT.h"
#include "Contact.h"
#include "imthread.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
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

void MainWindow::connectThread()
{
    connect(imThread, SIGNAL(contactDiscovered(QSharedPointer<Contact>)), contactList, SLOT(plantContact(QSharedPointer<Contact>)));
}
