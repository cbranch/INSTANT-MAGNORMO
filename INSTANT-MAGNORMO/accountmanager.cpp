#include "accountmanager.h"

AccountManager::AccountManager()
{
    accounts = QList<Account*>();
    gatherAccounts();
}

AccountManager::~AccountManager() {}

bool AccountManager::startDialog()
{
    accountmanagerdialog *managerDialog = new accountmanagerdialog();
    switch(managerDialog->exec()) {
        case QDialog::Accepted:
            activeUser = managerDialog->username;
            activePass = managerDialog->password;
            activeServer = managerDialog->server;
            activePort = managerDialog->port;
            writeAccounts();
            return true;
        case QDialog::Rejected:
        {
            printf("You suck balls\n");
            writeAccounts();
            return false;
        }
    }
    return false;
}

void AccountManager::writeAccounts()
{
    QSettings settings;
    settings.beginWriteArray("accounts");
    for(int i=0;i<accounts.size();++i) {
        settings.setArrayIndex(i);
        settings.setValue("user",accounts.at(i)->user);
        settings.setValue("password",accounts.at(i)->password);
        settings.setValue("server",accounts.at(i)->server);
        settings.setValue("port",accounts.at(i)->port);
        settings.setValue("type",accounts.at(i)->type);
        settings.setValue("active",accounts.at(i)->active);
    }
    settings.endArray();
}

void AccountManager::gatherAccounts()
{
    QSettings settings;
    int size = settings.beginReadArray("accounts");
    for(int i=0;i<size;++i) {
        Account *acc;
        acc->user = settings.value("user").toString();
        acc->password = settings.value("password").toString();
        acc->server = settings.value("server").toString();
        acc->port = settings.value("port").toInt();
        acc->type = (AccountType)settings.value("type").toInt();
        acc->active = settings.value("active").toBool();
        accounts.append(acc);
    }
    settings.endArray();
}
