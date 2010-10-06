#include "accountmanagerdialog.h"
#include "ui_accountmanagerdialog.h"

accountmanagerdialog::accountmanagerdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::accountmanagerdialog)
{
    ui->setupUi(this);
}

accountmanagerdialog::~accountmanagerdialog()
{
    delete ui;
}

void accountmanagerdialog::setAccountList(QList<Account*> accounts)
{
    QList<Account*>::iterator itr;
    for(itr = accounts.begin();itr < accounts.end(); ++itr) {
        Account *acc = *itr;
        ui->listWidget->addItem(makeListItem(acc));
    }
}

void accountmanagerdialog::on_buttonBox_accepted()
{
    QList<QListWidgetItem *> items = ui->listWidget->selectedItems();
    if(items.size()==0) {
        return;
    }
    QListWidgetItem *item = items.first();
    Account *acc = (Account*)qvariant_cast<void*>(item->data(Qt::UserRole));
    username = acc->user;
    password = acc->password;
    server = acc->server;
    port = acc->port;
}

void accountmanagerdialog::on_addButton_clicked()
{
    AccountDialog *accDiag = new AccountDialog();
    switch(accDiag->exec()) {
        case QDialog::Accepted:
        {
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
