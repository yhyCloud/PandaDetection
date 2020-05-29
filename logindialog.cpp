#pragma execution_character_set("utf-8")
#include "logindialog.h"
#include "mysqlite.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent), ui(new Ui::LoginDialog){

    ui->setupUi(this);
//    mysql = new MySqlite();
//    if(mysql->createDataBase("UserDB")){
//      qDebug() << "Database Create Sucessfully!";
//      if(mysql->CreateTableUser()){
//          qDebug() << QObject::tr("User表创建成功！\n");
//       }
//       else{
//          qDebug() << QObject::tr("User表创建失败！\n");
//       }
//    }
//    else
//    {
//       qDebug() << "Database Create Failed!";

//    }

}

void LoginDialog::on_LoginBtn_clicked()
{
    if(ui->lineEdit_Username->text().isEmpty() || ui->lineEdit_Password->text().isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("提示");
        msgBox.setText("请填写用户名与用户密码!");
        msgBox.exec();
        return ;
    }
    QString username = ui->lineEdit_Username->text();
    QString userPassword = ui->lineEdit_Password->text();

    QSqlDatabase db = QSqlDatabase::database("my_sql_connection"); //建立数据库连接
    QSqlQuery query(db);
    QString sqlStr = "Select * from User where Username = '"+username+"'";
    qDebug()<<sqlStr;
    bool isok=query.exec(sqlStr);

    if(!isok){
     qDebug()<<"查找数据失败"<<query.lastError().text();
     return;
    }
    if(isok){
        query.first();
        if(QString::compare(userPassword,query.value(2).toString(),Qt::CaseSensitive) == 0){
            QMessageBox msgBox;
            msgBox.setWindowTitle("提示");
            msgBox.setText("登录成功!");
            msgBox.exec();
            accept();
            return ;
        }
        else{
            QMessageBox msgBox;
            msgBox.setWindowTitle("提示");
            msgBox.setText("登录失败!");
            msgBox.exec();

            return ;
        }
    }
//    db.close();




}
