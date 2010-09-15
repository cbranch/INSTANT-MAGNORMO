#include "imthread.h"
#include "MAGNORMOBOT.h"

IMThread::IMThread(QString username, QString password) :
    username(username.toStdString()),
    password(password.toStdString())
{
}

void IMThread::run()
{
    bot = new MAGNORMOBOT();
    bot->start(username, password);
}
