#ifndef AccountManager_H
#define AccountManager_H

#include <QString>
#include <QList>
#include <QSettings>
#include "account.h"
#include "accountdialog.h"


class AccountManager
{
public:
    AccountManager();
    ~AccountManager();
    bool startDialog();

    // TODO: GET RID OF THESE AS THEY ARE ONLY A WORK AROUND FOR NOW
    QString activeUser;
    QString activePass;
    QString activeServer;
    int activePort;

private:
    void gatherAccounts();
    void writeAccounts();

    QList<Account> accounts;
};

#endif // AccountManager_H
