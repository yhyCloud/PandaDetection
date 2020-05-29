#pragma execution_character_set("utf-8")
#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include "mysqlite.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include "logindialog.h"
void setStyleSheet(/*QString styleSheetName*/){

    QFile file("E:\\QT_project\\Project01\\StyleSheet\\Style.qss");
    file.open(QIODevice::ReadOnly);
    QString css=file.readAll();
    qApp->setStyleSheet(css);
}
void initSQL(){

    /****************************初始化数据库************************************/
    MySqlite *mysql = new MySqlite;
    if(mysql->createDataBase("AnimalSearchDB")){
      qDebug() << "Database Create Sucessfully!";
      if(mysql->CreateTableUser()){
          qDebug() << QObject::tr("User表创建成功！\n");
       }
       else{
//          qDebug() << QObject::tr("User表创建失败！\n");


       }
    }
    else
    {
       qDebug() << "Database Create Failed!";

    }
    /*******************************************************************/

}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    setStyleSheet();
    initSQL();
//    LoginDialog logindialog;
//    if(logindialog.exec() == QDialog::Accepted){
     w.setWindowTitle("AnimalSearch");
        w.show();
        return a.exec();
//    }
//    w.show();

//    return a.exec();
}
