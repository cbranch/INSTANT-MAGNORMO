#include "accountmanagerdialog.h"
#include "ui_accountmanagerdialog.h"

accountmanagerdialog::accountmanagerdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::accountmanagerdialog)
{
    // Setup gui shit
    ui->setupUi(this);

    // Get account info
    readAccounts();
}

accountmanagerdialog::~accountmanagerdialog()
{
    delete ui;
}

void accountmanagerdialog::on_addButton_clicked()
{
    AccountDialog *accDiag = new AccountDialog(0);
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
            service = "FACEBOOK";
            break;
        case GAPP:
            service = "GAPP";
            break;
        case GTALK:
            service = "GTALK";
            break;
    }
    int snip = acc->user.indexOf("@");
    QString username = "";
    if(snip!=-1)
        username = acc->user.left(snip);
    QString itemName = service + " " + username;
    QListWidgetItem *accItem = new QListWidgetItem();
    accItem->setText(itemName);
    accItem->setData(Qt::UserRole,qVariantFromValue((void*)acc));
    if(acc->active) {
        accItem->setIcon(QIcon(":/icons/active"));
    } else {
        accItem->setIcon(QIcon(":/icons/inactive"));
    }

    return accItem;
}

Account* accountmanagerdialog::getAccount(QListWidgetItem *item)
{
    return (Account*)qvariant_cast<void*>(item->data(Qt::UserRole));
}

QList<Account*> accountmanagerdialog::getActiveAccounts()
{
    QList<Account*> activeAccounts = QList<Account*>();
    int numItems = ui->listWidget->count();
    for(int i=0;i<numItems;i++) {
        Account *acc = getAccount(ui->listWidget->item(i));
        if(acc->active)
            activeAccounts.append(acc);
    }

    return activeAccounts;
}

void accountmanagerdialog::writeAccounts()
{
    QSettings settings;
    settings.beginWriteArray("accounts");
    settings.remove("");
    int numItems = ui->listWidget->count();
    for(int i=0;i<numItems;i++) {
        settings.setArrayIndex(i);
        Account *acc = getAccount(ui->listWidget->item(i));
        settings.setValue("user",acc->user);
        settings.setValue("password",acc->password);
        settings.setValue("server",acc->server);
        settings.setValue("port",acc->port);
        settings.setValue("type",acc->type);
        settings.setValue("active",acc->active);
    }
    settings.endArray();
}

void accountmanagerdialog::readAccounts()
{
    QSettings settings;
    int size = settings.beginReadArray("accounts");
    for(int i=0;i<size;++i) {
        settings.setArrayIndex(i);
        Account *acc = new Account();
        acc->user = settings.value("user").toString();
        acc->password = settings.value("password").toString();
        acc->server = settings.value("server").toString();
        acc->port = settings.value("port").toInt();
        acc->type = (AccountType)settings.value("type").toInt();
        acc->active = settings.value("active").toBool();

        // Here we make the list widget item containing each account
        ui->listWidget->addItem(makeListItem(acc));
    }
    settings.endArray();
}

void accountmanagerdialog::on_buttonBox_accepted()
{
    writeAccounts();
}

void accountmanagerdialog::on_editButton_clicked()
{
    QList<QListWidgetItem*> selected = ui->listWidget->selectedItems();
    if(selected.isEmpty())
        return;
    QListWidgetItem *thisOne = selected.first();
    Account *acc = getAccount(thisOne);

    AccountDialog *accDiag = new AccountDialog(0,acc);
    accDiag->exec();
}

void accountmanagerdialog::on_toggleButton_clicked()
{
    QList<QListWidgetItem*> selected = ui->listWidget->selectedItems();
    if(selected.isEmpty())
        return;
    QListWidgetItem *thisOne = selected.first();
    Account *acc = getAccount(thisOne);

    if(acc->active) {
        acc->active = false;
        thisOne->setIcon(QIcon(":/icons/inactive"));
    } else {
        acc->active = true;
        thisOne->setIcon(QIcon(":/icons/active"));
    }
}

void accountmanagerdialog::on_removeButton_clicked()
{
    QListWidgetItem *thisItem = ui->listWidget->currentItem();
    ui->listWidget->takeItem(ui->listWidget->currentRow());
    delete thisItem;
}
