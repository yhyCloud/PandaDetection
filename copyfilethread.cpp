#include "copyfilethread.h"
#include <QFile>

void CopyFileThread::run()
{
    CopyDir.replace('/',QChar('\\'));
    qDebug()<<"进入线程CopyFileThread:"<<CopyDir;


     QString CSVFile = "D:\\AnimalSearch\\TableCSV.csv";
     QFile file(CSVFile);
     if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
     {
        qDebug()<<"Cannot open file for reading: ";
        return;
     }
     QStringList list;
     list.clear();
     QTextStream in(&file);
     int i =0;
     while(!in.atEnd())
     {
        QString fileLine = in.readLine();
        list = fileLine.split(",", QString::SkipEmptyParts);

        QString TargetFileName;
        TargetFileName = QString("%1\\%2.jpg").arg(CopyDir).arg(i);
        qDebug()<<"拷贝文件"<<list.at(0)<<"到"<<TargetFileName;
        QFile::copy(list.at(0),TargetFileName);
        i++;
      }

     file.close();
     emit CopySuccess();
//     QMessageBox msgBox;
//     msgBox.setWindowTitle("提示");
//     msgBox.setText("拷贝完毕！");
//     msgBox.exec();


}

void CopyFileThread::RecevieCopyFileName(QString fileName)
{
    CopyDir = fileName;

}
