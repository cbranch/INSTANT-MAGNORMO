#ifndef IMTHREAD_H
#define IMTHREAD_H

#include <QThread>
#include <QString>

class MAGNORMOBOT;

class IMThread : public QThread
{
public:
    IMThread(QString username, QString password);

    void run();

    std::string username;
    std::string password;
    MAGNORMOBOT *bot;
};

#endif // IMTHREAD_H
