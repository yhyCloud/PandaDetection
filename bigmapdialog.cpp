#pragma execution_character_set("utf-8")
#include "bigmapdialog.h"
#include "ui_bigmapdialog.h"
//#include<mainwindow.h>
#include <QDebug>
#include <QVariant>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QAxObject>
#include <QMessageBox>
#include <QGridLayout>
#include <QBoxLayout>
#include <QPushButton>
#include <QLayout>
BigMapDialog::BigMapDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BigMapDialog)
{

    ui->setupUi(this);
    InitTable();
    connect(ui->tableView,SIGNAL(clicked(QModelIndex)),this,SLOT(table_view_row_clicked(QModelIndex)));
//    importCSVFile_Gaode();
    InitMap();
    QGridLayout *mainLayout=new QGridLayout(this);
    mainLayout->addWidget(ui->tableView,0,0);
    mainLayout->addWidget(m_pWebView,0,1);




//    m_mainwindow=new MainWindow;
//    connect(m_mainwindow,SIGNAL(SendSignalToBigMapDialog(QString,QString,QString)),this,SLOT(receiveJingWeiData(QString,QString,QString)));

//    LoadMap();
}

BigMapDialog::~BigMapDialog()
{
    delete ui;
}

void BigMapDialog::LoadMap()
{

    //加载网页地图
//    ui->webWidget->setControl(QString::fromUtf8("{8856F961-340A-11D0-A96B-00C04FD705A2}"));
//    ui->webWidget->setProperty("DisplayAlerts",false);  //不显示任何警告信息
//    ui->webWidget->setFocusPolicy(Qt::StrongFocus);//设置控件接收键盘焦点的方式：鼠标单击、Tab键。
//    ui->webWidget->setProperty("DisplayScrollBars",false); // 显示滚动条
//    ui->webWidget->setProperty("fullScreen",true);
//    ui->webWidget->dynamicCall("Navigate(const QString&)","file:///E:/dingwei_v3.html");
}

void BigMapDialog::InitMap()
{
    m_pWebView=new QWebEngineView(this);
    m_pWebView->load(QUrl::fromLocalFile("D:/AnimalSearch/GaodeMapDemo.html"));
    m_pWebView->showMaximized();
}

void BigMapDialog::InitTable()
{
    //init table view
    table_model_SearchResult = new QStandardItemModel();
    table_model_SearchResult->setColumnCount(6);
    table_model_SearchResult->setHeaderData(0,Qt::Horizontal,tr("检索成功文件名"));
    table_model_SearchResult->setHeaderData(1,Qt::Horizontal,tr("所处文件夹位置"));
    table_model_SearchResult->setHeaderData(2,Qt::Horizontal,tr("识别可能性"));
    table_model_SearchResult->setHeaderData(3,Qt::Horizontal,tr("识别种类"));
    table_model_SearchResult->setHeaderData(4,Qt::Horizontal,tr("经度"));
    table_model_SearchResult->setHeaderData(5,Qt::Horizontal,tr("纬度"));
    //二、设置表格属性：
    ui->tableView->setModel(table_model_SearchResult);
    //设置表隔行变色
    ui->tableView->setAlternatingRowColors(true);
    //设置每一次选中的是一整行也不是一个单独的item
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //表头信息显示居左
    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);
    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    //设置列宽可伸缩
    ui->tableView->horizontalHeader()->setSectionResizeMode (0,QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode (1,QHeaderView::Stretch);
    ui->tableView->setColumnWidth(0,101);
    ui->tableView->setColumnWidth(1,102);
    //设置选中时为整行选中
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //设置表格的单元为只读属性，即不能编辑
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //如果你用在QTableView中使用右键菜单，需启用该属性
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
}

void BigMapDialog::importCSVFile_Gaode()
{
    //表格要先清空
    table_model_SearchResult->clear();
    QString fileName = "D:\\AnimalSearch\\detectionresult.csv" ;
    QFile file(fileName);

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
        int CurrentRow=table_model_SearchResult->rowCount();
            for(int item = 0; item < 6; item++)
            {
                    table_model_SearchResult->setItem(CurrentRow, item, new QStandardItem(list.at(item)));
                    table_model_SearchResult->item(CurrentRow, item)->setForeground(QBrush(QColor(0, 0, 0)));
            }


            i++;
        }

     file.close();
     qDebug()<<"大地图上导入detection.csv文件";
}

void BigMapDialog::receiveJingWeiData(QString JingValue, QString WeiVaule,QString path)
{
   qDebug()<<"接受到信号数据"<<"+"<<JingValue<<"+"<<WeiVaule<<path;
    Jing = JingValue;
    Wei  = WeiVaule;
    Path="file://"+path;
    Path="'"+Path;
    Path=Path+"'";
    qDebug()<<"bigmap输出"<<Path<<"..."<<Jing<<"..."<<Wei;
//    receiveJingWeiData;
//    qDebug()<<"Jing-Wei"<<"+"<<Jing<<"+"<<Wei;
//    ui->lineEdit_jing->setText(Jing);
//    ui->lineEdit_wei->setText(Wei);
//    QAxObject *document = ui->webWidget->querySubObject("Document");
//    QAxObject *parentWindow = document->querySubObject("parentWindow");
//  parentWindow->dynamicCall("execScript(QString,QString)",QString("qtLocation(%1,%2)").arg(Jing).arg(Wei),"JavaScript");
//    parentWindow->dynamicCall("getPicPath(const QString&,const QString&,const QString&)",JingValue,WeiVaule,path ); //调用参数
//    parentWindow->dynamicCall("qtLocation(const QString&,const QString&,const QString&)",JingValue,WeiVaule,path ); //调用参数
    m_pWebView->page()->runJavaScript(QString("getPicPath_Gaode(%1,%2,%3)").arg(Jing).arg(Wei).arg(Path));
    qDebug()<<"shsdfasdf*****************---------+++";
}

void BigMapDialog::table_view_row_clicked(const QModelIndex &index)
{
    QAbstractItemModel  *modessl = ui->tableView->model();
    int curRow  = index.row();
    //结果图片地址
    QModelIndex indextemp01 = modessl->index(curRow,1);//遍历第一行的所有列
    //这个是一个单元格的值。tostring()----ok
    QString datatemp01 = modessl->data(indextemp01).toString();
    datatemp01.replace('/',QChar('\\'));
//        datatemp01.replace('\\',QRegExp("\\/"));
    qDebug()<<"datatemp01:"<<datatemp01;
    QPixmap image;
    image.load(datatemp01);
     if(!(image.isNull()))
     {
         MapPath = datatemp01;
         qDebug()<<"这是图片文件"<<datatemp01;
     }
     else{
         qDebug()<<"这bu是图片文件"<<datatemp01;
     }

    //经度
    QModelIndex indextemp02 = modessl->index(curRow,4);//遍历第一行的所有列
    //这个是一个单元格的值。tostring()----ok
    QString datatemp02 = modessl->data(indextemp02).toString();
    qDebug()<<"datatemp02:"<<datatemp02;

    //维度
    QModelIndex indextemp03 = modessl->index(curRow,5);//遍历第一行的所有列
    //这个是一个单元格的值。tostring()----ok
    QString datatemp03 = modessl->data(indextemp03).toString();
    qDebug()<<"datatemp03:"<<datatemp03;


//    QAxObject *document = ui->MapWidget->querySubObject("Document");
//    QAxObject *parentWindow = document->querySubObject("parentWindow");
      datatemp01="file://"+datatemp01;
      qDebug()<<"测试输出01"<<datatemp01;
//      QString test01;

      qDebug()<<"测试输出01"<<datatemp01;
      QString test01 = "'"+datatemp01;
      QString test02 =test01+"'";
      qDebug()<<"测试输出02"<<test02;
     test02.replace('\\',('/'));
//      m_pWebView->page()->runJavaScript(QString("showtest(%1)").arg(test02));
//      m_pWebView->page()->runJavaScript("getPicPath_Gaode(%1,)",datatemp01); //调用参数
     m_pWebView->page()->runJavaScript(QString("getPicPath_Gaode(%1,%2,%3)").arg(datatemp02).arg(datatemp03).arg(test02));
     //调用一个函数
//     m_pWebView->page()->runJavaScript(QString("getArrayMarkNew()"));
//    parentWindow->dynamicCall("execScript(QString,QString)",QString("DragOpen()"),"JavaScript");
//    parentWindow->dynamicCall("execScript(QString,QString)",QString("getPicPath_Gaode(%1,%2,%3)").arg(datatemp02).arg(datatemp03).arg(datatemp01),"JavaScript");
    //parentWindow->dynamicCall("getPicPath_Gaode(const QString&,const QString&,const QString&)",datatemp02,datatemp03,datatemp01 ); //调用参数

}

