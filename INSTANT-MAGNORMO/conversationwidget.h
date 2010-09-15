#ifndef CONVERSATIONWIDGET_H
#define CONVERSATIONWIDGET_H

#include <QWidget>
#include <QString>

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

public slots:

private:
    Ui::ConversationWidget *ui;

public:
    QString jid;

};

#endif // CONVERSATIONWIDGET_H
