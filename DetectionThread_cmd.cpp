#include "DetectionThread_cmd.h"
#include <QProcess>
#include <QDebug>

DetectionThread_cmd::DetectionThread_cmd(QObject *parent) : QThread(parent)
{

}

void DetectionThread_cmd::run()
{
    QProcess process(this);
    qDebug()<<"执行cmd图像检测";
    process.startDetached("cmd",QStringList()<<"/c"<<"python D:/AnimalSearch/Pandadetection/detection_v2.py");
    process.waitForStarted();
    process.waitForFinished(-1);
}
