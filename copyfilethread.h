#ifndef COPYFILETHREAD_H
#define COPYFILETHREAD_H

#include <QThread>
#include <QDebug>
#include <QString>
class CopyFileThread : public QThread
{
    Q_OBJECT
public:

    explicit CopyFileThread(QObject *parent=0):QThread(parent){

        qDebug()<<"Progress Bar Thread:"<< QThread::currentThreadId();
//        write();

    }
    virtual void run();
    QString CopyDir;
public slots:
    void RecevieCopyFileName(QString fileName);
signals:
    void CopySuccess();
};

#endif // COPYFILETHREAD_H
