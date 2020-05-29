#ifndef ADDUSERDIALOG_H
#define ADDUSERDIALOG_H
#include "ui_adduserdialog.h"
#include "mysqlite.h"
namespace Ui {
    class AddUserDialog;
}

class AddUserDialog : public QDialog
{
    Q_OBJECT
public:
//    AddUserDialog();
    explicit AddUserDialog(QWidget *parent = 0);
private slots:


    void on_AdUserdBtn_clicked();

    void on_DelUserBtn_clicked();

private:
    Ui::AddUserDialog *ui;
    MySqlite *mysql;
signals:
    void Signal1_Update();//自己定义的一个信号--让mainwindow中的表格从数据库中读取数据然后显示在表格中
};

#endif // ADDUSERDIALOG_H
