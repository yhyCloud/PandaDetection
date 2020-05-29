#ifndef WRITETHREAD_H
#define WRITETHREAD_H
#include <QThread>

class WriteThread : public QThread
{
public:
    WriteThread(QString datas);
    virtual void run();
    QString filedata;
};

#endif // WRITETHREAD_H
