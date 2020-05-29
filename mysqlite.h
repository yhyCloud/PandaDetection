#ifndef MYSQLITE_H
#define MYSQLITE_H

#include <QString>
class MySqlite
{
public:
    MySqlite();
    bool createDataBase(const QString &dbName);//创建数据库文件
    bool CreateTableUser();//创建数据库表User
    bool CreateTableModel();//创建数据库表Model
    bool inserUsertData(const QString &username,const QString &userpwd,const QString &userrole,const QString &description);//为User表插入数据
    bool UpdateUserData(const QString  &id,const QString &username,const QString &userpwd,const QString &userrole,const QString &description);//为User表更新数据
//    bool inserModelData(const QString &username,const QString &userpwd,const QString &userrole,const QString &description);//为Model表插入数据
    bool IsTableExist(QString strTableName);
};

#endif // MYSQLITE_H
