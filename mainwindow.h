#pragma once
#pragma execution_character_set("utf-8")

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDirModel>
#include <QMenu>
#include <QFileSystemModel>
#include <QDir>
#include <QMap>
#include <QStringList>
#include <QString>
#include <QFile>
#include <QStandardItemModel>
#include <QFileSystemWatcher>
#include <DetectionThread.h>
#include <QTime>
#include <QStandardItemModel>
#include <progressbarthread.h>
#include "adduserdialog.h"
#include "updateuserdialog.h"
#include "bigmapdialog.h"
#include "copyfilethread.h"
#include <QAxWidget>
#include <VideodetectThread.h>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <customslider.h>
#include <QTimer>
#include <QWebEngineView>
#include <DetectionThread_cmd.h>
#include <Videodetectionthread_cmd.h>

namespace Ui {
class MainWindow;
}

class BigMapDialog;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void init_UI();
    void getDataBaseModel();//获取的是新建模型数据表中的数据
    void initTreeView();
    void CreatMenu();
    void initTableView();
//    void initSQL();//初始化数据库
    void TestTableSearchResult();//这个只是用来测试一下某一个功能 稍后删
    void initUserTable();// 初始化用户列表 从数据库中读取用户表 然后显示出来

//    void Init_ProgressBar();//进度条初始化
    void updateUserTable();
    void loadMap();
    void sleep(int msec);
    int clickFlag;
    int videostartFlag;
    int picstartFlag;
    int barcnt;
    int piccnt=0;
    void initVideoTreeView();
private slots:
    void on_NewModel_Clicked();
    void on_AnimalSreach_Clicked();
    void on_OptimizeModel_Clicked();
    void on_UserManage_Clicked();
    void on_PhotoDeal_Clicked();
    void on_Start_Clicked();//启动主界面


    void showPath(QString path);//获取新添加图片路径
    void showResultPic(QString path);//显示结果图片
    void showOrigPic(QString path);//显示原始图片
    void showDetctionresult(QString path);//显示检索相关结果
    void setmyBarvalue(QString path);

    void table_view_row_clicked(const QModelIndex &index);
    void tableview_picfile_clicked(const QModelIndex &index);
    void importCSVFile(QString path);
//    //新建模型(没用)
//    void on_NewModel_1_Clicked();//模型设置
//    void on_NewModel_2_Clicked();//新建模型
//    void on_NewModel_3_Clicked();//训练设置
//    //动物检索(没用)
//    void on_AnimalSreach_1_Clicked();
//    void on_AnimalSreach_2_Clicked();
//    //优化模型(没用)
//    void on_OptimizeModel_1_Clicked();//模型选择
//    void on_OptimizeModel_2_Clicked();//训练集标注
//    void on_OptimizeModel_3_Clicked();//训练设置
//    //用户管理(没用)
//    void on_UserManage_1_Clicked();
//    void on_UserManage_2_Clicked();
    //图像处理(没用)


    void mkdir();//新建文件
    void rm();//删除文件
    void AddSearch();//将文件夹中的图片或者单独图片文件加入检索表格
    void onSelectLabelText();//点击获取当前目录
    void DelTableRow();//删除表格中的任意行
    void on_lineEdit_textChanged(const QString &arg1);//搜索表格中的指定行
    void on_StartSearchBtn();//开始检索
    void on_lineEdit_Threshold_textChanged(const QString &arg1);//检索界面-检索结果表 按照阈值大小来筛选图片
    void on_pushButton_ResultSort_clicked();//检索结果排序按钮
    void on_NewModel_NextBtn1_Clicked();//新建模型页面的模型设置的下一步按钮

    void handleResults(int value);//更新进度条
    void startProgressBarThread();//开启进度条线程

    void on_ShowDataBase();//更新数据库用户表信息


    void on_DelUserBtn_clicked();

    void on_AddUserBtn_clicked();

    void on_UpdateUserBtn_clicked();


    void SetBigMap();
    void CopyFileBtn();
    void CopySuccessFunction();
    void tableView_SearchFile_clicked(const QModelIndex &index);//点击检测结果列表，显示原图

    void addVideoSearch();//将视频文件加入检索
    void on_StartVideoSearch();//开始视频文件检索
    void tableview_Videofile_clicked(const QModelIndex &index);
    void tableview_resultVideofile_clicked(const QModelIndex &index);



    void on_origvideopause_btn_clicked();

    void on_origvideoend_btn_clicked();

    void on_resultvideopause_btn_clicked();

    void on_resultvideoend_btn_clicked();

    void importpresntvideoResult(QString);//向视频结果表格中添加数据

    void on_DelVideoFile_clicked();

    void onorigTimerout();//原始视频的计时器
    void onresultTimerout();//结果视频的计时器

    void origslider_clicked();//点击原始视频进度条
    void resultslider_clicked();//点击结果视频进度条

    void origslider_moved();//拖动原始视频进度条
    void resultslider_moved();//拖动结果视频进度条

    void origslider_released();//释放原始视频进度条
    void resultslider_released();//释放结果视频进度条

    void setVideoProgressbar();

    void initMap();//初始化地图界面
    void init_Bigmaptable();
    void importcsvfileGaode();
    void bigmap_table_row_clicked(const QModelIndex &index);
    void bigmap_receiveJingWeiData(QString Jingdu,QString Weidu,QString path);

    void onorigPositionChanged(qint64 position);
    void onorigDurationChanged(qint64 duration);
    void on_origslider_valueChanged(int value);

    void onresultPositionChanged(qint64 position);
    void onresultDurationChanged(qint64 duration);


    void on_resultslider_valueChanged(int value);

    void on_setnewVideoSearchbtn_clicked();

    void on_setnewPicSearch_clicked();

protected:
    void contextMenuEvent(QContextMenuEvent *event);//添加菜单事件
private:
    Ui::MainWindow *ui;
    QFileSystemModel *model;
    QFileSystemModel *videomodel;
    QStandardItemModel *table_model;
    QStandardItemModel *table_model_SearchFile;
    QStandardItemModel *table_model_SearchResult;
    QStandardItemModel *table_model_UserList;
    QStandardItemModel *table_model_UserList_tab2;
    QStandardItemModel *table_model_videoSearch;//检索设置界面视频文件表格
    QStandardItemModel *table_model_videoFile;//检索结果界面视频文件表格
    QStandardItemModel *table_model_resultVideoFile;//检索结果界面结果视频文件表格
    QStandardItemModel *table_model_bigmap;
    QMenu *m_MainMenu;

    QAction *m_AddFileDirAction;
    QAction *m_DelAction;
    QAction *m_SearchAction;
    ProgressBarThread *m_progressBarThread;//进度条线程
    AddUserDialog *dialog;
    UpdateUserDialog *updateDialog;

    DetectionThread* mythread;//检测线程对象
    DetectionThread_cmd* cmd_mythread;//图像检测cmd线程对象
    Videodetectionthread_cmd* cmd_videothread;//视频检测cmd线程对象
    QFileSystemWatcher *myWatcher;//QFileSystemWatcher变量
    QFileSystemWatcher *videoWatcher;//QFileSystemWatcher变量，监控视频检索结果变化
    QMap<QString, QStringList> m_currentContentsMap; // 当前每个监控的内容目录列表
    QMap<QString, QStringList> video_currentContentsMap;//视频监控的内容目录列表
    QString filename;
    QStringList piclist;//存储结果图片的列表
    QStringList piclist2;
    BigMapDialog *bigMapDialog;
    CopyFileThread *copyfileThread;
    QString CurrentPath=NULL;
    VideodetectThread* videothread;
    QMediaPlayer *origplayer;
    QMediaPlayer *resultplayer;
    QVideoWidget *origVideoWidget;
    QVideoWidget *resultVidoWidget;

    QTimer *origvideoTimer;
    QTimer *resultvideoTimer;
    QTimer *videoprogressTimer;
    QWebEngineView* big_WebView;
    QString MapPath;
    QString Jing;
    QString Wei;
    QString Path;

    int maxValue=1000;
    bool if_origreload=false;
    bool if_resultreload=false;



signals:
    void SendSignalToUpdateUserDialog(int id,QString username,QString userpwd,QString userrole,QString description);
    void sendFilepath(QString path);//发送新增文件路径
    void sendVideoFilepath(QString path);//发送新增视频文件路径
    void sendValuesOfFiles(int counts);//发送一共需要检索多少行
    void sendCompleteOneOfPic();//检索完毕一张就发送一个信号到进度条

    void  SendSignalToBigMapDialog(QString JingValue,QString WeiValue,QString path);
    void  SendCopyFileName(QString fileName);
    void  fangda();//用于importcsvfile_gaode





};

#endif // MAINWINDOW_H
