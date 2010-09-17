#ifndef CONVERSATIONWIDGET_H
#define CONVERSATIONWIDGET_H

#include <QWidget>
#include <QString>
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

protected:
    void changeEvent(QEvent *e);

signals:
    void sendMessage(QString jid, QString msg);

public slots:
    void messageRevieved(QString msg, QString jid);

private:
    Ui::ConversationWidget *ui;

public:
    QString jid;


private slots:
    void on_lineEdit_returnPressed();
};

#endif // CONVERSATIONWIDGET_H
