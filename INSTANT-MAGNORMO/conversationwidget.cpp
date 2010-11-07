#include "conversationwidget.h"
#include "ui_conversationwidget.h"
#include <QtGui/QApplication>

ConversationWidget::ConversationWidget(QString jid, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConversationWidget),
    jid(jid)
{
    ui->setupUi(this);
    connect(ui->textEdit,SIGNAL(anchorClicked(QUrl)),this,SLOT(handleClickedLink(QUrl)));
}

ConversationWidget::~ConversationWidget()
{
    delete ui;
}

QString ConversationWidget::getRichMessage(QString original)
{
	QRegExp linkify("((?:http|https|ftp)://\\S*)");
	original.replace(linkify, tr("<a href=\"\\1\">\\1</a>"));
	QRegExp linkifyFuzzy("\b(www\.\\S*)");
	original.replace(linkifyFuzzy, tr("<a href=\"\\1\">\\1</a>"));

	return original;
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
        ui->textEdit->append(getRichMessage(msg));
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
    if(message=="") return;
    ui->lineEdit->setText(QString(""));
    emit sendMessage(jid,message);
}

void ConversationWidget::on_pushButton_clicked()
{
    // Tell the perp that we are doing some screenshottin or somefink
    ui->textEdit->append("...SPYING ON YOU...");

    // Get the screen shot
    QPixmap screenShot = QPixmap::grabWindow(QApplication::desktop()->winId());

    // Set up the network stuff for hitting up imgur
    QNetworkAccessManager *nam = new QNetworkAccessManager(this);
    connect(nam,SIGNAL(finished(QNetworkReply*)),this,SLOT(finishedImageUpload(QNetworkReply*)));

    // Convert screenshot to bytearray
    QByteArray imageBytes;
    QBuffer buffer(&imageBytes);
    buffer.open(QIODevice::WriteOnly);
    screenShot.save(&buffer,"PNG");

	QString crlf("\r\n");
	QString boundaryString("----------------991719012056841984089743414380");
	QString boundary = "--" + boundaryString + crlf;
	QByteArray multipart;
	multipart.append(boundary);
	multipart.append(QString("Content-Disposition: form-data; name=\"image\"; filename=\"screenshot.png\"" + crlf));
	multipart.append(QString("Content-Type: image/png" + crlf + crlf));
	multipart.append(imageBytes);
	multipart.append(QString(crlf + "--" + boundaryString + "--" + crlf));

	// imgur anonymooooos api url
    QString url("http://api.imgur.com/2/upload.xml");
	QString query("?key=6ac90b313a7090ef63a05de4acef2418");
	QNetworkRequest request(QUrl(url + query));
	request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data; boundary=" + boundaryString);
    nam->post(request, multipart);
}

void ConversationWidget::finishedImageUpload(QNetworkReply *reply)
{
    // This function deals with the reply from imgur
    QXmlStreamReader xml(reply);

    while(!xml.atEnd()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        if(token==QXmlStreamReader::StartElement) {
            if(xml.name()=="original") {
                xml.readNext();
                emit sendMessage(jid, tr("CHECK IT OUT: ") + xml.text().toString());
                return;
            }
        }
    }

    // If we make it this far it didnt work
    ui->textEdit->append("...that totally didnt work...");
}

void ConversationWidget::handleClickedLink(QUrl link)
{
    QDesktopServices::openUrl(link);
}

