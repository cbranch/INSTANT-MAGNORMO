#include "conversationwidget.h"
#include "ui_conversationwidget.h"

ConversationWidget::ConversationWidget(QString jid, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConversationWidget),
    jid(jid)
{
    ui->setupUi(this);
}

ConversationWidget::~ConversationWidget()
{
    delete ui;
}

void ConversationWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ConversationWidget::messageRevieved(QString msg, QString jid)
{
    if(jid==this->jid) {
        ui->textEdit->append(msg);
    }
}

void ConversationWidget::onVisibleChange(bool visible)
{
    if(!visible)
        emit dissapearWindow(jid);
}

void ConversationWidget::on_lineEdit_returnPressed()
{
    QString message = ui->lineEdit->text();
    ui->lineEdit->setText(QString(""));
    emit sendMessage(jid,message);
}
