#include "conversationwidget.h"
#include "ui_conversationwidget.h"
#include <QtGui/QApplication>
#include <QTimer>

ConversationWidget::ConversationWidget(QString jid, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConversationWidget),
    jid(jid),
	stoppedTypingTimer(new QTimer(this)),
	inactiveTimer(new QTimer(this))
{
    ui->setupUi(this);
    connect(ui->textEdit,SIGNAL(anchorClicked(QUrl)),this,SLOT(handleClickedLink(QUrl)));
	stoppedTypingTimer->setSingleShot(true);
	inactiveTimer->setSingleShot(true);
	connect(stoppedTypingTimer, SIGNAL(timeout()), SLOT(userStoppedTyping()));
	connect(inactiveTimer, SIGNAL(timeout()), SLOT(userIsInactive()));
}

ConversationWidget::~ConversationWidget()
{
    delete ui;
}

QString ConversationWidget::getRichMessage(QString original)
{
	QRegExp linkify("((?:http|https|ftp)://\\S*)");
	original.replace(linkify, tr("<a href=\"\\1\">\\1</a>"));
	QRegExp linkifyFuzzy("\b(www\\.\\S*)");
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

void ConversationWidget::onMessageChanged()
{
	lastChatState = gloox::ChatStateInvalid;
	if (ui->textEdit->toPlainText().size() == 0) {
		lastChatState = gloox::ChatStateActive;
	} else {
		lastChatState = gloox::ChatStateComposing;
	}
	emit chatStateChanged(jid, lastChatState);
	stoppedTypingTimer->start(5000);
	inactiveTimer->stop();
}

void ConversationWidget::userStoppedTyping()
{
	if (lastChatState == gloox::ChatStateComposing) {
		lastChatState = gloox::ChatStatePaused;
		emit chatStateChanged(jid, lastChatState);
	}
	inactiveTimer->start(10000);
}

void ConversationWidget::userIsInactive()
{
	lastChatState = gloox::ChatStateInactive;
	emit chatStateChanged(jid, lastChatState);
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

void ConversationWidget::giveMeVCard(QString jid, MagnormoCard vcard)
{
	ui->contactPhoto->setPixmap(vcard.photo);
}

