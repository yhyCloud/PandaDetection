#ifndef PROGRESSBARTHREAD_H
#define PROGRESSBARTHREAD_H
#include <QThread>
#include <QDebug>
#include <QFile>

class ProgressBarThread : public QThread
{
    Q_OBJECT
public:
    explicit ProgressBarThread(QObject *parent=0):QThread(parent){

        qDebug()<<"%%%%%%%%%%Progress Bar Thread:%%%%%%%%"<< QThread::currentThreadId();


    }
    void write(){
    }
protected:
    virtual void run() Q_DECL_OVERRIDE{
        //每隔50毫秒更新当前值，然后发射resultReady()信号（用于更新进度条）。
        qDebug()<<"&&&&&&&&&&&&&&&&Progress Bar Run Thread:&&&&&&&&&&&&7"<< QThread::currentThreadId();
        qDebug()<<"&&&&&&&&&&&&&&&&dealNums=&&&&&&&&&&&&&&&&"<<dealNums;
        qDebug()<<"&&&&&&&&&&&&&&&&filesNum=&&&&&&&&&&&&&&&&"<<filesNum;
        int nValue = 0;
        while(dealNums <= filesNum){
            //休眠50毫秒
            msleep(100);
            qDebug()<<"while-dealNums="<<dealNums;
            qDebug()<<"while-filesNum="<<filesNum;
            nValue= (dealNums/filesNum) *100;
            //准备更新
            emit resultReady(nValue);
        }
    }
signals:
    void resultReady(int value);
private slots:
    void recieveValuesOfFiles(int num);
    void recieveNumsOfPics();
public:
    int filesNum=0;
    int dealNums=0;
};

#endif // PROGRESSBARTHREAD_H
