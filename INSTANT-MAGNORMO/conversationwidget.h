#ifndef CONVERSATIONWIDGET_H
#define CONVERSATIONWIDGET_H

#include <QWidget>
#include <QApplication>
#include <QDesktopWidget>
#include <QDesktopServices>
#include <QString>
#include <QPixmap>
#include <QFileDialog>
#include <QDir>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QByteArray>
#include <QBuffer>
#include <QIODevice>
#include <QList>
#include <QXmlStreamReader>
#include <cstdio>

namespace Ui {
    class ConversationWidget;
}

class ConversationWidget : public QWidget
{
Q_OBJECT
public:
    explicit ConversationWidget(QString jid, QWidget *parent = 0);
    ~ConversationWidget();

	QString getRichMessage(QString original);

protected:
    void changeEvent(QEvent *e);

signals:
    void sendMessage(QString jid, QString msg);
    void dissapearWindow(QString jid);

public slots:
    void messageRevieved(QString msg, QString jid);
    void onVisibleChange(bool visible);

private:
    Ui::ConversationWidget *ui;

public:
    QString jid;


private slots:
    void on_pushButton_clicked();
    void on_lineEdit_returnPressed();
    void finishedImageUpload(QNetworkReply *reply);
    void handleClickedLink(QUrl link);
};

#endif // CONVERSATIONWIDGET_H
