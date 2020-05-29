#pragma execution_character_set("utf-8")
#include "mysqlite.h"
#include <qsqldatabase.h>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

MySqlite::MySqlite()
{

}

bool MySqlite::createDataBase(const QString &dbName)
{
    //    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        QSqlDatabase db =  QSqlDatabase::addDatabase("QSQLITE", "my_sql_connection");
        db.setDatabaseName(dbName);
        if (!db.open()) {
            qDebug() << "Database Error!";
            return false;
        }
        db.close();
        return true;

}

bool MySqlite::CreateTableUser()
{
    QSqlDatabase db = QSqlDatabase::database("my_sql_connection"); //建立数据库连接
    //第一句话的参数 QT默认的是qt_sql_default_connection  当然你也可以再之前addDatabase的时候第二个参数随便指定 然后这里的参数就是
    //之前的第二个参数的值
    QSqlQuery query(db);

    bool success = query.exec("CREATE TABLE User ("
                              "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                              "Username VARCHAR(30),"
                              "Userpwd VARCHAR(100),"
                              "Userrole varchar(50),"
                              "Description varchar(200))");
    if(success)
    {
        qDebug() << QObject::tr("数据库表创建成功！\n");
        db.close();
        return true;
    }
    else
    {
        qDebug() << QObject::tr("数据库表创建失败！\n");
        bool istableExist = IsTableExist("User");
        if(istableExist){
            qDebug()<<"User表创建失败，因为User表已经存在了";
        }
        else{
            qDebug()<<"User表创建失败，且User表不存在了";
        }
        return false;
    }

}

bool MySqlite::CreateTableModel()
{
//    QSqlDatabase db = QSqlDatabase::database("my_sql_connection"); //建立数据库连接
//    //第一句话的参数 QT默认的是qt_sql_default_connection  当然你也可以再之前addDatabase的时候第二个参数随便指定 然后这里的参数就是
//    //之前的第二个参数的值
//    QSqlQuery query(db);

//    bool success = query.exec("CREATE TABLE Model ("
//                              "id INTEGER PRIMARY KEY AUTOINCREMENT,"
//                              "Username VARCHAR(30),"
//                              "Userpwd VARCHAR(100),"
//                              "Userrole varchar(50),"
//                              "Description varchar(200))");
//    if(success)
//    {
//        qDebug() << QObject::tr("数据库表创建成功！\n");

//        return true;
//    }
//    else
//    {
//        qDebug() << QObject::tr("数据库表创建失败！\n");
//        return false;
//    }
    return 0;
}

bool MySqlite::inserUsertData(const QString &username,const QString &userpwd,const QString &userrole,const QString &description)
{
    QSqlDatabase db = QSqlDatabase::database("my_sql_connection"); //建立数据库连接
    //第一句话的参数 QT默认的是qt_sql_default_connection  当然你也可以再之前addDatabase的时候第二个参数随便指定 然后这里的参数就是
    //之前的第二个参数的值
    QSqlQuery query(db);
//    qDebug()<<"username:"<<username<<"userpwd:"<<userpwd<<"userrole:"<<userrole<<"Description:"<<description;
    QString sqlStr = "insert into User (Username,Userpwd,Userrole,Description) values ('"

            +username+"','"

            +userpwd+"','"

            +userrole+"','"

            +description+"')";
//    qDebug()<<sqlStr;
    bool isok=query.exec(sqlStr);

   if(!isok){
     qDebug()<<"数据写入失败"<<db.lastError().text();;
     return false;
   }
   db.close();
   return true;

}

bool MySqlite::UpdateUserData(const QString  &id,const QString &username, const QString &userpwd, const QString &userrole, const QString &description)
{
     QSqlDatabase db = QSqlDatabase::database("my_sql_connection"); //建立数据库连接
     QSqlQuery query(db);
     QString sqlStr = "Update User set Username = '"+username+"',"+"Userpwd ='"+userpwd+"',"+"Userrole='"+userrole+"',"+"Description='"+description+"'"+" Where id='"+id+"'";
     qDebug()<<sqlStr;
     bool isok=query.exec(sqlStr);

    if(!isok){
      qDebug()<<"更新数据失败"<<db.lastError().text();;
      return false;
    }
    db.close();
    return true;

}

bool MySqlite::IsTableExist(QString strTableName)
{

    QSqlDatabase db = QSqlDatabase::database("my_sql_connection"); //建立数据库连接
    QSqlQuery query(db);
    bool isTableExist=query.exec(QString("select count(*) from sqlite_master where type='table' and name='%1'").arg(strTableName));    //关键的判断

                db.close();
                return isTableExist;

}
