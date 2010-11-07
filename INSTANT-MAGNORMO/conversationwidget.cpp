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

    QNetworkAccessManager *nam = new QNetworkAccessManager(this);
    connect(nam,SIGNAL(finished(QNetworkReply*)),this,SLOT(finishedImageUpload(QNetworkReply*)));

    // Convert screenshot to bytearray
    QByteArray imageBytes;
    QBuffer buffer(&imageBytes);
    buffer.open(QIODevice::WriteOnly);
    screenShot.save(&buffer,"PNG");

    QUrl url("http://api.imgur.com/2/upload.xml");
    QByteArray data;
    data.append(QString("image=").toUtf8());
    data.append(imageBytes.toBase64());
    data.append(QString("&key=6ac90b313a7090ef63a05de4acef2418").toUtf8());
    nam->post(QNetworkRequest(url),data);
}

void ConversationWidget::finishedImageUpload(QNetworkReply *reply)
{
    QXmlStreamReader xml(reply->readAll());

    while(!xml.atEnd()) {
        xml.readNext();
        // Find the image link here
    }

    QList<QByteArray> headerList = reply->rawHeaderList();
    QList<QByteArray>::iterator itr;
    for(itr=headerList.begin();itr<headerList.end();++itr) {
        QByteArray bytes = *itr;
        QString stuff(bytes);
        qDebug(stuff.toStdString().c_str());
    }
}
