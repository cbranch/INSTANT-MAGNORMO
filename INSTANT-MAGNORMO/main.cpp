#include <QtGui/QApplication>
#include <QFile>
#include <QTextStream>
#include "mainwindow.h"
#include <stdio.h>
#include "MAGNORMOBOT.h"
#include "Contact.h"
#include "QAppBar.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	qRegisterMetaType<Contact>("Contact");
    qRegisterMetaType<QSharedPointer<Contact> >("QSharedPointer<Contact>");

    QCoreApplication::setOrganizationName("MAGNORMO");
    QCoreApplication::setOrganizationDomain("joshfyne.com");
    QCoreApplication::setApplicationName("INSTANT_MAGNORMO");

    MainWindow w;
    w.startAccountManager(false);
    w.show();
	
	// This code shows how to put the main window into a right-docked app bar
	/*QAppBar sample(QAppBarEdgeRight, 250);
	sample.setWindowTitle("INSTANT MAGNORMO");
	sample.getContentArea()->addWidget(&w);
	sample.show();*/
	
    return a.exec();
}
