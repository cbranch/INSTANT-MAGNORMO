#include <QtGui/QApplication>
#include <QFile>
#include <QTextStream>
#include "mainwindow.h"
#include <stdio.h>
#include "MAGNORMOBOT.h"
#include "Contact.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<QSharedPointer<Contact> >("QSharedPointer<Contact>");

    QCoreApplication::setOrganizationName("MAGNORMO");
    QCoreApplication::setOrganizationDomain("joshfyne.com");
    QCoreApplication::setApplicationName("INSTANT_MAGNORMO");

	{
		QFile styleSheet(":/styles.css");
		if (styleSheet.open(QFile::ReadOnly)) {
			QTextStream out(&styleSheet);
			qApp->setStyleSheet(out.readAll());
		}
	}

    MainWindow w;
    w.startAccountManager(false);
    w.show();
    return a.exec();
}
