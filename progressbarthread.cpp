#include "progressbarthread.h"

//ProgressBarThread::ProgressBarThread()
//{

//}

void ProgressBarThread::recieveValuesOfFiles(int num)
{
    qDebug()<<"before-filesNum:"<<filesNum;
    filesNum = num;
    qDebug()<<"after-filesNum:"<<filesNum;
}

void ProgressBarThread::recieveNumsOfPics()
{
    qDebug()<<"before-dealNums:"<<dealNums;
    dealNums++;
    qDebug()<<"after-dealNums:"<<dealNums;

}
