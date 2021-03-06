#include <QtGui/QApplication>
#include <QFile>
#include <QTextStream>
#include "mainwindow.h"
#include <stdio.h>
#include "MAGNORMOBOT.h"
#include "Contact.h"
#include "QAppBar.h"
#include "vcard.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	qRegisterMetaType<Contact>("Contact");
    qRegisterMetaType<QSharedPointer<Contact> >("QSharedPointer<Contact>");
	qRegisterMetaType<MagnormoCard>("MagnormoCard");

    QCoreApplication::setOrganizationName("MAGNORMO");
    QCoreApplication::setOrganizationDomain("joshfyne.com");
    QCoreApplication::setApplicationName("INSTANT_MAGNORMO");

    MainWindow w;
    w.startAccountManager(false);

    w.show();

#ifdef WIN32
	// This code shows how to put the main window into a right-docked app bar
	QAppBar sample(QAppBarEdgeRight, 250);
	sample.setWindowTitle("INSTANT MAGNORMO");
	sample.getContentArea()->addWidget(&w);
	sample.setAttribute(Qt::WA_QuitOnClose);
	sample.show();
#endif	

    return a.exec();
}
