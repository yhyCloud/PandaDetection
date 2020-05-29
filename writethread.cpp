#pragma execution_character_set("utf-8")
#include "writethread.h"
#include <QFile>
#include <QDebug>
WriteThread::WriteThread(QString datas)
{
    filedata=datas;
    qDebug()<<"WriteThread-filedata"<<filedata;
}

void WriteThread::run()
{
    //数据写入文件

//    QFile file("E:\filePath.txt");
//    file.open(QIODevice::WriteOnly | QIODevice::Text);
//    file.write(filedata.toUtf8());
//    file.close();

//                 QFile file("E:\\filePath.txt");
//                 if(!file.open(QIODevice::WriteOnly|QIODevice::Append))
//                 {
//                    qDebug()<<"单元格内容:Open failed.";

//                 }
//                 else{
//                     file.write(filedata.toUtf8());
//                     file.close();
//                 }

}


