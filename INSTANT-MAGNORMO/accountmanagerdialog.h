#ifndef ACCOUNTMANAGERDIALOG_H
#define ACCOUNTMANAGERDIALOG_H

#include <QDialog>

namespace Ui {
    class accountmanagerdialog;
}

class accountmanagerdialog : public QDialog
{
    Q_OBJECT

public:
    explicit accountmanagerdialog(QWidget *parent = 0);
    ~accountmanagerdialog();

private:
    Ui::accountmanagerdialog *ui;
};

#endif // ACCOUNTMANAGERDIALOG_H
