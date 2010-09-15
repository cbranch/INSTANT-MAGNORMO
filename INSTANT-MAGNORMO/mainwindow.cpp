#include "mainwindow.h"
#include <QDockWidget>
#include "contactlist.h"

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
