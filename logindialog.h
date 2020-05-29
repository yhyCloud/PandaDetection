#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H


#include"ui_logindialog.h"
#include "mysqlite.h"
namespace Ui {
    class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT
public:

    explicit LoginDialog(QWidget *parent = 0);
private slots:
    void on_LoginBtn_clicked();

private:
    Ui::LoginDialog *ui;
    MySqlite *mysql;
};

#endif // LOGINDIALOG_H
