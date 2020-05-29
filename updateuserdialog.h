#ifndef UPDATEUSERDIALOG_H
#define UPDATEUSERDIALOG_H

#include "ui_updateuserdialog.h"
#include "mysqlite.h"

namespace Ui {
    class UpdateUserDialog;
}


class UpdateUserDialog : public QDialog
{
    Q_OBJECT
public:
//    UpdateUserDialog();
    explicit UpdateUserDialog(QWidget *parent = 0);
private slots:
    void on_UpdateUserdBtn_clicked();

    void on_DelUserBtn_clicked();

    void receiveMainWindowData(int id,QString username,QString userpwd,QString userrole,QString description);

private:
    Ui::UpdateUserDialog *ui;
    MySqlite *mysql;


signals:
    void Signal_Send();//自己定义的一个信号--让mainwindow中的表格从数据库中读取数据然后显示在表格中
};

#endif // UPDATEUSERDIALOG_H
