#include "accountmanagerdialog.h"
#include "ui_accountmanagerdialog.h"

accountmanagerdialog::accountmanagerdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::accountmanagerdialog)
{
    accounts = QList<Account *>();

    // Setup gui shit
    ui->setupUi(this);

    // Get account info
    gatherAccounts();

    // Build the List box stuff
    QList<Account*>::iterator itr;
    for(itr = accounts.begin();itr < accounts.end(); ++itr) {
        Account *acc = *itr;
        ui->listWidget->addItem(makeListItem(acc));
    }
}

accountmanagerdialog::~accountmanagerdialog()
{
    delete ui;
}

void accountmanagerdialog::on_addButton_clicked()
{
    AccountDialog *accDiag = new AccountDialog();
    switch(accDiag->exec()) {
        case QDialog::Accepted:
        {
            accounts.append(accDiag->account);
            ui->listWidget->addItem(makeListItem(accDiag->account));
            break;
        }
        case QDialog::Rejected:
            return;
    }
}

QListWidgetItem* accountmanagerdialog::makeListItem(Account *acc)
{
    QString service = "FUCKING SOMETHING WRONG";
    switch(acc->type) {
        case FACEBOOK:
            service = "BOOK OF FACES";
            break;
        case GAPP:
            service = "GAPP";
            break;
        case GTALK:
            service = "GTALK";
            break;
    }
    QString itemName = service + " " + acc->user;
    QListWidgetItem *accItem = new QListWidgetItem();
    accItem->setText(itemName);
    accItem->setData(Qt::UserRole,qVariantFromValue((void*)acc));

    return accItem;
}

Account* accountmanagerdialog::getAccount(QListWidgetItem *item)
{
    return (Account*)qvariant_cast<void*>(item->data(Qt::UserRole));
}

QList<Account*> accountmanagerdialog::getActiveAccounts()
{
    QList<Account*> activeAccounts = QList<Account*>();
    QList<Account*>::iterator itr;
    for(itr = accounts.begin(); itr<accounts.end(); ++itr) {
        Account *acc = *itr;
        if(acc->active)
            activeAccounts.append(acc);
    }

    return activeAccounts;
}

void accountmanagerdialog::writeAccounts()
{
    printf("WRITING ACCOUNTS\n");
    fflush(stdout);
    QSettings settings;
    settings.beginWriteArray("accounts");
    QList<Account*>::iterator itr;
    int i=0;
    for(itr=accounts.begin();itr<accounts.end();++itr) {
        settings.setArrayIndex(i);
        settings.setValue("user",accounts.at(i)->user);
        settings.setValue("password",accounts.at(i)->password);
        settings.setValue("server",accounts.at(i)->server);
        settings.setValue("port",accounts.at(i)->port);
        settings.setValue("type",accounts.at(i)->type);
        settings.setValue("active",accounts.at(i)->active);
        i++;
    }
    settings.endArray();
}

void accountmanagerdialog::gatherAccounts()
{
    printf("READING ACCOUNTS\n");
    fflush(stdout);
    QSettings settings;
    int size = settings.beginReadArray("accounts");
    for(int i=0;i<size;++i) {
        Account *acc = new Account();
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

void accountmanagerdialog::on_buttonBox_accepted()
{
    writeAccounts();
}
