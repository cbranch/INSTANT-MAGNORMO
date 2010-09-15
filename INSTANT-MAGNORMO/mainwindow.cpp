#include "mainwindow.h"
#include <QDockWidget>
#include "contactlist.h"
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
}

MainWindow::~MainWindow()
{
}

void MainWindow::connectThread()
{
    connect(imThread, SIGNAL(contactDiscovered(QSharedPointer<Contact>)), contactList, SLOT(plantContact(QSharedPointer<Contact>)));
}
