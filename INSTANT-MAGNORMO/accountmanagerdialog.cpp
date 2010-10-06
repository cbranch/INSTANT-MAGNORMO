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
