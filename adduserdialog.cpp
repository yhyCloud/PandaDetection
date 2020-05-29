#pragma execution_character_set("utf-8")
#include "adduserdialog.h"
 #include <QDebug>
#include <QMessageBox>
AddUserDialog::AddUserDialog(QWidget *parent) : QDialog(parent), ui(new Ui::AddUserDialog){

    ui->setupUi(this);
    mysql = new MySqlite();


}

void AddUserDialog::on_AdUserdBtn_clicked()
{
        //获取用户信息
        //如果没有填写任何信息就点击添加--报错
        //添加即为加入数据库中
        //然后添加完毕之后 完毕对话框 并在mainwindow中的表格中显示
        if(ui->lineEdit_Username->text().isEmpty() || ui->lineEdit_UserPwd->text().isEmpty()|| ui->lineEdit_Userrole->text().isEmpty()
                || ui->lineEdit_Description->text().isEmpty())
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle("提示");
            msgBox.setText("信息填写不完整,请完善用户信息!");
            msgBox.exec();
            return ;
        }

        if(!mysql->inserUsertData(ui->lineEdit_Username->text(), ui->lineEdit_UserPwd->text(),ui->lineEdit_Userrole->text(),ui->lineEdit_Description->text())){
            QMessageBox msgBox;
            msgBox.setWindowTitle("提示");
            msgBox.setText("添加出错");
            msgBox.exec();
            return ;
        }
        else{
            QMessageBox msgBox;
            msgBox.setWindowTitle("提示");
            msgBox.setText("添加用户成功");
            msgBox.exec();
            //关闭dialog框
            //刷新mainwindow中的表

            emit Signal1_Update();//发射信号 让数据显示在表格中
            qDebug()<<"信号发送";
    //        QDialog::accept();//关闭对话框

        }
}

void AddUserDialog::on_DelUserBtn_clicked()
{
    QDialog::accept();//关闭对话框
}
