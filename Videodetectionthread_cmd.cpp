#include "Videodetectionthread_cmd.h"
#include <QProcess>
#include <QDebug>

Videodetectionthread_cmd::Videodetectionthread_cmd(QObject *parent) : QThread(parent)
{

}

void Videodetectionthread_cmd::run()
{
    QProcess videoprocess(this);
    videoprocess.start("cmd",QStringList()<<"/c"<<"python D:/AnimalSearch/Pandadetection/videodetect.py");
    qDebug()<<"执行视频检测";
    videoprocess.waitForStarted();
    videoprocess.waitForFinished(-1);
}
