#include "conversationwidget.h"
#include "ui_conversationwidget.h"
#include <QtGui/QApplication>

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
        qApp->alert(this,3000);
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

void ConversationWidget::on_pushButton_clicked()
{
    QPixmap screenShot = QPixmap::grabWindow(QApplication::desktop()->winId());

    QString format = "png";
    QString initialPath = QDir::currentPath() + tr("/untitled.") + format;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                                    initialPath,
                                                    tr("%1 Files (*.%2);;All Files (*)")
                                                    .arg(format.toUpper())
                                                    .arg(format));
    if (!fileName.isEmpty())
        screenShot.save(fileName, format.toAscii());
}
