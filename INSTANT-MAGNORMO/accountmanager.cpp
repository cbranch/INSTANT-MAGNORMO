#include "accountmanager.h"

AccountManager::AccountManager()
{
    allAccounts = QList<Account>();
    activeAccounts = QList<Account>();
    gatherAccounts();
}

AccountManager::~AccountManager() {}

bool AccountManager::startDialog()
{
    AccountDialog *account = new AccountDialog();
    switch (account->exec()) {
    case QDialog::Accepted:
        activeUser = account->username;
        activePass = account->password;
        activeServer = account->server;
        activePort = account->port;
        return true;
    case QDialog::Rejected:
        {
        printf("You suck balls\n");
        return false;
        }
    }
}

void AccountManager::writeAccounts()
{
    QSettings settings;
    settings.beginWriteArray("accounts");
    for(int i=0;i<accounts.size();++i) {
        settings.setArrayIndex(i);
        settings.setValue("user",accounts.at(i).user);
        settings.setValue("password",accounts.at(i).password);
        settings.setValue("type",accounts.at(i).type);
        settings.setValue("active",accounts.at(i).active);
    }
    settings.endArray();
}

void AccountManager::gatherAccounts()
{
    QSettings settings;
    int size = settings.beginReadArray("accounts");
    for(int i=0;i<size;++i) {
        Account acc;
        acc.user = settings.value("user").toString();
        acc.password = settings.value("password").toString();
        acc.type = (AccountType)settings.value("type").toInt();
        acc.active = settings.value("active").toBool();
        accounts.append(acc);
    }
    settings.endArray();
}
