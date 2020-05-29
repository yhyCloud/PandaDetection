#pragma execution_character_set("utf-8")
#include "updateuserdialog.h"
#include <QDebug>
#include <QMessageBox>

UpdateUserDialog::UpdateUserDialog(QWidget *parent) : QDialog(parent), ui(new Ui::UpdateUserDialog){

    ui->setupUi(this);
    mysql = new MySqlite();




}

void UpdateUserDialog::on_UpdateUserdBtn_clicked()
{
    if(ui->lineEdit_Username->text().isEmpty() || ui->lineEdit_UserPwd->text().isEmpty()|| ui->lineEdit_Userrole->text().isEmpty()
            || ui->lineEdit_Description->text().isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("提示");
        msgBox.setText("信息填写不完整,请完善用户信息!");
        msgBox.exec();
        return ;
    }
    if(!mysql->UpdateUserData(ui->label_ID->text(),ui->lineEdit_Username->text(), ui->lineEdit_UserPwd->text(),ui->lineEdit_Userrole->text(),ui->lineEdit_Description->text())){
        QMessageBox msgBox;
        msgBox.setWindowTitle("提示");
        msgBox.setText("更新出错");
        msgBox.exec();
        return ;
    }
    else{
        QMessageBox msgBox;
        msgBox.setWindowTitle("提示");
        msgBox.setText("更新用户成功");
        msgBox.exec();
        //关闭dialog框
        //刷新mainwindow中的表

        emit Signal_Send();//发射信号 让数据显示在表格中
        qDebug()<<"信号发送";
        QDialog::accept();//关闭对话框

    }
}

void UpdateUserDialog::on_DelUserBtn_clicked()
{
    QDialog::accept();
}

void UpdateUserDialog::receiveMainWindowData(int id, QString username, QString userpwd, QString userrole, QString description)
{
    qDebug()<<"接受到信号数据"<<id<<"+"<<username<<"+"<<userpwd<<"+"<<userrole<<"+"<<description;
    ui->lineEdit_Username->setText(username);
//    QString labelText =id;
    ui->label_ID->setText(QString::number(id));
    ui->lineEdit_UserPwd->setText(userpwd);
    ui->lineEdit_Userrole->setText(userrole);
    ui->lineEdit_Description->setText(description);
    qDebug()<<"填写信号数据完毕";

}
