#ifndef STATUSWIDGET_H
#define STATUSWIDGET_H

#include <QWidget>

class QProgressBar;
class QLabel;

class StatusWidget : public QWidget
{
Q_OBJECT
public:
    explicit StatusWidget(QWidget *parent = 0);

signals:

public slots:

protected:
    QLabel *label;
    QProgressBar *progress;

};

#endif // STATUSWIDGET_H
