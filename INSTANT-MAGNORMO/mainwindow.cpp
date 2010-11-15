#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    // Set style sheet here
    setTheme(":/styles/default.magstyle");

    // Docking shit
    setDockOptions(QMainWindow::AllowNestedDocks | QMainWindow::AllowTabbedDocks | QMainWindow::AnimatedDocks);

    // Contact List
    contactListDock = new QDockWidget(tr("Contacts"), this);
    contactList = new ContactList();
    contactListDock->setWidget(contactList);
    addDockWidget(Qt::LeftDockWidgetArea, contactListDock);
    connect(contactList, SIGNAL(conversationInitiated(Contact)), SLOT(startConversation(Contact)));

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

	readSettings();
}

MainWindow::~MainWindow()
{
}

void MainWindow::connectAccount(QString username, QString password, QString server, int port)
{
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

void MainWindow::startConversation(Contact contact)
{
	MAGNORMOBOT *bot = contact.conduit.data();
	QString jid = contact.jid;
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
			connect(convo, SIGNAL(chatStateChanged(QString, gloox::ChatStateType)), bot, SLOT(updateChatState(QString, gloox::ChatStateType)));
			connect(bot, SIGNAL(vCardReceived(QString, MagnormoCard)), convo, SLOT(giveMeVCard(QString, MagnormoCard)));

			bot->getVCardFromJid(jid);
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
                QList<Account*> activeAccounts = accMgrDiag->getAccounts();
                if(activeAccounts.size()==0) {
                    printf("You suck balls\n");
                    return;
                }
                QList<Account*>::iterator itr;
                for(itr=activeAccounts.begin();itr<activeAccounts.end();++itr) {
                    Account* acc = *itr;
                    if(acc->active)
                        connectionManager->connectAccount(acc);
                    else
                        connectionManager->disconnectAccount(acc);
                }
                return;
            }
            case QDialog::Rejected:
            {
                return;
            }
        }
    } else {
        QList<Account*> activeAccounts = accMgrDiag->getActiveAccounts();
        QList<Account*>::iterator itr;
        for(itr=activeAccounts.begin();itr<activeAccounts.end();++itr) {
            Account* acc = *itr;
            connectionManager->connectAccount(acc);
        }
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

	// Window menu
    QAction *closeAction = toolBar->addAction(QIcon(":/icons/close"),"BE A QUITTER");
	QApplication *app = static_cast<QApplication *>(QCoreApplication::instance());
    connect(closeAction, SIGNAL(triggered()), app, SLOT(quit()));
}

void MainWindow::getAccountManager()
{
    startAccountManager(true);
}

void MainWindow::getContactWindow()
{
    contactListDock->setVisible(true);
}

void MainWindow::setTheme(QString themeLocation)
{
    QFile theme(themeLocation);
    if (theme.open(QFile::ReadOnly)) {
        QTextStream out(&theme);
        qApp->setStyleSheet(out.readAll());
    }
}

void MainWindow::readSettings()
{
	QSettings settings;
	restoreGeometry(settings.value("geometry").toByteArray());
	restoreState(settings.value("windowState").toByteArray());
}

void MainWindow::writeSettings()
{
	QSettings settings;
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	writeSettings();
    QMainWindow::closeEvent(event);
}