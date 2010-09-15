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
