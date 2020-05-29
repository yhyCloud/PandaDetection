#pragma execution_character_set("utf-8")
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_bigmapdialog.h"
#include "bigmapdialog.h"

#include <QDebug>
#include <QPixmap>
#include <QFont>
#include <QIcon>
#include <tabwidget.h>
#include <QTreeView>
#include <QHeaderView>
#include <QHBoxLayout>//布局类
#include <QString>//文本类
#include <QPushButton>
#include <QInputDialog>
#include <QMessageBox>
#include <QDirModel>
#include <QDebug>
#include <QPixmap>
#include <QFileSystemModel>
#include <QStandardItemModel>
#include <writethread.h>
#include <DetectionThread.h>
#include <qsqldatabase.h>
#include <QSqlQuery>
#include <QSqlError>
#include <QTextStream>
#include <QStringList>
#include <QImage>
#include <QString>
#include <QFile>
#include <QAxObject>
#include <QFileDialog>
#include <QProcess>
#include <Windows.h>
#include <QTabWidget>
#include <QScrollBar>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
//    ui->CurrentDirLabel->setMaximumWidth(500);
//    ui->CurrentFileLabel->setMaximumWidth(200);
    //开始默认设置stackedWidget的界面
    ui->stackedWidget_main->setCurrentIndex(1);
    ui->tabWidget_UserManage->setTabPosition(QTabWidget::West);
    ui->tabWidget_UserManage->tabBar()->setStyle(new CustomTabStyle);
//    ui->tabWidget_NewModel->setTabPosition(QTabWidget::West);
//    ui->tabWidget_NewModel->tabBar()->setStyle(new CustomTabStyle);
    ui->tabWidget_AnimalSearch->setTabPosition(QTabWidget::West);
    ui->tabWidget_AnimalSearch->tabBar()->setStyle(new CustomTabStyle);
//    ui->tabWidget_OPtimizeModel->setTabPosition(QTabWidget::West);
//    ui->tabWidget_OPtimizeModel->tabBar()->setStyle(new CustomTabStyle);
    ui->tabWidget->setTabPosition(QTabWidget::West);
    ui->tabWidget->tabBar()->setStyle(new CustomTabStyle);

    //设置阈值lineEdit只能输入数字0-100之间的整数
    QRegExp regx("[0-9]+$");
    QValidator *validator = new QRegExpValidator(regx, ui->lineEdit_Threshold );
    ui->lineEdit_Threshold->setValidator( validator );
    //按钮不显示边框
    ui->pushButton_ResultSort->setFlat(true);


    resultVidoWidget=new QVideoWidget;
    ui->resultVideo->addWidget(resultVidoWidget);


    origplayer=new QMediaPlayer;
    origplayer->setNotifyInterval(200);//播放器信息更新间隔
    origVideoWidget=new QVideoWidget;

    ui->OrigVideo->addWidget(origVideoWidget);
    origplayer->setVideoOutput(origVideoWidget);
    ui->origslider->setEnabled(false);
    ui->origvideopause_btn->setEnabled(false);
    ui->origvideoend_btn->setEnabled(false);

    ui->setnewVideoSearchbtn->setEnabled(false);//在进入系统时，不能直接新建检索任务
    ui->setnewPicSearch->setEnabled(false);//在进入系统时，不能直接新建检索任务

    init_UI();
    initMap();
    getDataBaseModel();
    initTreeView();
    initTableView();
    init_Bigmaptable();
    initVideoTreeView();
    CreatMenu();
    connect(ui->FiletreeView,SIGNAL(clicked(QModelIndex)),this,SLOT(onSelectLabelText()));
    //TestTableSearchResult();//这个只是用来测试一下某一个功能 稍后删
    clickFlag=0;
    videostartFlag=0;//定义程序执行的flag，初始为0，用来控制是否需要新建

    dialog = new AddUserDialog;//这里在初始化的时候 不能为new AddUserDialog()括号不能加！！！！！！！
    updateDialog = new UpdateUserDialog;
    connect(dialog,SIGNAL(Signal1_Update()),this,SLOT(on_ShowDataBase()));
    connect(updateDialog,SIGNAL(Signal_Send()),this,SLOT(on_ShowDataBase()));
    connect(this,SIGNAL(SendSignalToUpdateUserDialog(int ,QString ,QString ,QString ,QString )),updateDialog,SLOT(receiveMainWindowData(int,QString,QString ,QString ,QString )));
    //告知有多少个检索文件数
//    m_progressBarThread = new ProgressBarThread();
//    connect(this, SIGNAL(sendValuesOfFiles(int)), m_progressBarThread, SLOT(recieveValuesOfFiles(int)));
//    connect(this, SIGNAL(sendCompleteOneOfPic()), m_progressBarThread, SLOT(recieveNumsOfPics()));

//    bigMapDialog = new BigMapDialog;//这里在初始化的时候 不能为new BigMapDialog()括号不能加！！！！！！！
//    connect(this,SIGNAL(SendSignalToBigMapDialog(QString ,QString ,QString)),bigMapDialog,SLOT(receiveJingWeiData(QString,QString,QString)));
//    connect(this,SIGNAL(fangda()),bigMapDialog,SLOT(importcsvfileGaode()));

    connect(this,SIGNAL(SendSignalToBigMapDialog(QString,QString,QString)),this,SLOT(bigmap_receiveJingWeiData(QString,QString,QString)));
    connect(this,SIGNAL(fangda()),this,SLOT(importcsvfileGaode()));
    connect(ui->bigmap_table,SIGNAL(clicked(QModelIndex)),this,SLOT(bigmap_table_row_clicked(QModelIndex)));

    connect(ui->startVideoSearch,SIGNAL(clicked(bool)),this,SLOT(on_StartVideoSearch()));
    connect(ui->VideofileTableview,SIGNAL(clicked(QModelIndex)),this,SLOT(tableview_Videofile_clicked(QModelIndex)));
    connect(ui->VideoresultTableview,SIGNAL(clicked(QModelIndex)),this,SLOT(tableview_resultVideofile_clicked(QModelIndex)));
    connect(ui->tableView,SIGNAL(clicked(QModelIndex)),this,SLOT(tableview_picfile_clicked(QModelIndex)));

//    connect(ui->origslider,&CustomSlider::costomSliderClicked,this,&MainWindow::origslider_clicked);
//    connect(ui->origslider,&CustomSlider::sliderMoved,this,&MainWindow::origslider_moved);
//    connect(ui->origslider,&CustomSlider::sliderReleased,this,&MainWindow::origslider_released);

//    connect(ui->resultslider,&CustomSlider::costomSliderClicked,this,&MainWindow::resultslider_clicked);
//    connect(ui->resultslider,&CustomSlider::sliderMoved,this,&MainWindow::resultslider_moved);
//    connect(ui->resultslider,&CustomSlider::sliderReleased,this,&MainWindow::resultslider_released);

    connect(origplayer,SIGNAL(positionChanged(qint64)),this,SLOT(onorigPositionChanged(qint64)));
    connect(origplayer,SIGNAL(durationChanged(qint64)),this,SLOT(onorigDurationChanged(qint64)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init_UI()
{
   //ICON
    QString iconPath("../AnimalSearch/icon/timg.jpg");


    QPixmap image;
    image.load(iconPath);
    if(!(image.isNull())){
        image.scaled(ui->label_Icon->size(),Qt::KeepAspectRatio);
        ui->label_Icon->setPixmap(image);//加载到Label标签
        ui->label_Icon->setScaledContents(true);
        ui->label_Icon->show();//显示
    }
    else
    {
        qDebug()<<"没有图片";
    }
    //Title
    QFont font( "Microsoft YaHei", 18, 45);//第一个属性是字体（微软雅黑），第二个是大小，第三个是加粗（权重是75）

    ui->label_Title->setFont(font);
    //Tool Button(4个)


    //进度条初始化
    ui->progressBar_AnimalSearch->setFixedHeight(25);
    ui->progressBar_AnimalSearch->setRange(0,100);
    ui->progressBar_AnimalSearch->setValue(0);

    ui->progressBar->setRange(0,100);
    ui->progressBar->setValue(0);

    // 连接信号槽
    //点击了开始检索之后就应该开启进度条的线程了...

//    connect(ui->StartSearchBtn, SIGNAL(clicked(bool)), this, SLOT(startProgressBarThread()));
//    ui->tabWidget_AnimalSearch->setTabEnabled(2,false);
//      ui->tab_5->setEnabled(false);
//      ui->tab_7->setEnabled(false);


}

void MainWindow::getDataBaseModel()
{
//    //从数据库获取的类型填充到QCombobox控件中
//    ui->comboBox_ChooseModel->addItem(tr("识别模型选择"));
//    ui->comboBox_ChooseModel->setToolTip(tr("sasas"));
//    ui->comboBox_ChooseModel->addItem(tr("Faster RCNN"));
//    ui->comboBox_ChooseModel->addItem(tr("SSD"));
//    ui->comboBox_ChooseModel->addItem(tr("YOLO"));

//    //
//    ui->comboBox_ChooseAnimal->addItem(tr("识别动物类型选择"));
//    ui->comboBox_ChooseAnimal->addItem(tr("大熊猫"));
//    ui->comboBox_ChooseAnimal->addItem(tr("小浣熊"));
//    ui->comboBox_ChooseAnimal->addItem(tr("眼镜蛇"));
      qDebug()<<"暂无选择";
}

void MainWindow::initTreeView()
{
    /******************************************************/
    model = new QFileSystemModel;
    model->setReadOnly(false);            //设置可以修改
    model->setRootPath(QDir::currentPath());
    QStringList filter;
    filter<<"*.jpg"<<".png"<<"*.jpeg";
    model->setNameFilters(filter);
//    model->setFilter(QDir::Files);
    model->setNameFilterDisables(false);

    ui->FiletreeView->setModel(model);

    ui->FiletreeView->header()->setStretchLastSection(true);
    ui->FiletreeView->header()->setSortIndicator(0, Qt::AscendingOrder);
    ui->FiletreeView->header()->setSortIndicatorShown(true);
    //treeView->header()->s(true);

    QModelIndex index = model->index(QDir::currentPath());
    ui->FiletreeView->expand(index);      //当前项展开
    ui->FiletreeView->scrollTo(index);    //定位到当前项
    ui->FiletreeView->resizeColumnToContents(0);
    /******************************************************/



//    connect(ui->createBtn, SIGNAL(clicked()), this, SLOT(mkdir()));
//    connect(ui->delBtn, SIGNAL(clicked()), this, SLOT(rm()));
    connect(ui->AddSearchBtn, SIGNAL(clicked()), this, SLOT(AddSearch()));
}

void MainWindow::CreatMenu()
{
    m_MainMenu=new QMenu("操作菜单");;
    m_AddFileDirAction=new QAction("新建文件夹");
    m_DelAction=new QAction("删除");
    m_SearchAction=new QAction("加入检索");
    m_MainMenu->addAction(m_AddFileDirAction);
    m_MainMenu->addAction(m_DelAction);
    m_MainMenu->addAction(m_SearchAction);

    connect(m_AddFileDirAction,SIGNAL(triggered(bool)),this,SLOT(mkdir()));
    connect(m_DelAction,SIGNAL(triggered(bool)),this,SLOT(rm()));
    connect(m_SearchAction,SIGNAL(triggered(bool)),this,SLOT(AddSearch()));
}

void MainWindow::initTableView()
{
    //初始化表头
    //一、添加表头：

    table_model = new QStandardItemModel();
    table_model->setColumnCount(2);
    table_model->setHeaderData(0,Qt::Horizontal,tr("文件名"));
    table_model->setHeaderData(1,Qt::Horizontal,tr("文件路径"));

    table_model_SearchFile = new QStandardItemModel();
    table_model_SearchFile->setColumnCount(2);
    table_model_SearchFile->setHeaderData(0,Qt::Horizontal,tr("文件名"));
    table_model_SearchFile->setHeaderData(1,Qt::Horizontal,tr("文件路径"));


    table_model_SearchResult = new QStandardItemModel();
    table_model_SearchResult->setColumnCount(7);
    table_model_SearchResult->setHeaderData(0,Qt::Horizontal,tr("检索成功文件名"));  
    table_model_SearchResult->setHeaderData(1,Qt::Horizontal,tr("所处文件夹位置"));
    table_model_SearchResult->setHeaderData(2,Qt::Horizontal,tr("识别可能性"));
    table_model_SearchResult->setHeaderData(3,Qt::Horizontal,tr("识别种类"));
    table_model_SearchResult->setHeaderData(4,Qt::Horizontal,tr("经度"));
    table_model_SearchResult->setHeaderData(5,Qt::Horizontal,tr("纬度"));
    table_model_SearchResult->setHeaderData(6,Qt::Horizontal,tr("原始图片文件位置"));

    table_model_videoSearch =new QStandardItemModel();//视频检索设置界面表格
    table_model_videoSearch->setColumnCount(2);
    table_model_videoSearch->setHeaderData(0,Qt::Horizontal,tr("文件名"));
    table_model_videoSearch->setHeaderData(1,Qt::Horizontal,tr("文件路径"));

    table_model_videoFile=new QStandardItemModel();//视频检索界面原始文件表格
    table_model_videoFile->setColumnCount(2);
    table_model_videoFile->setHeaderData(0,Qt::Horizontal,tr("文件名"));
    table_model_videoFile->setHeaderData(1,Qt::Horizontal,tr("文件路径"));

    table_model_resultVideoFile=new QStandardItemModel();//视频检索界面检索结果表格
    table_model_resultVideoFile->setColumnCount(3);
    table_model_resultVideoFile->setHeaderData(0,Qt::Horizontal,tr("文件名"));
    table_model_resultVideoFile->setHeaderData(1,Qt::Horizontal,tr("出现时间"));
    table_model_resultVideoFile->setHeaderData(2,Qt::Horizontal,tr("结果视频路径"));



    //二、设置表格属性：

    ui->tableView->setModel(table_model);
    ui->tableView_SearchFile->setModel(table_model_SearchFile);
    ui->tableView_SearchResult->setModel(table_model_SearchResult);
    ui->VideosearchTableView->setModel(table_model_videoSearch);
    ui->VideofileTableview->setModel(table_model_videoFile);
    ui->VideoresultTableview->setModel(table_model_resultVideoFile);

    //设置表隔行变色
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView_SearchFile->setAlternatingRowColors(true);
    ui->tableView_SearchResult->setAlternatingRowColors(true);
    ui->VideosearchTableView->setAlternatingRowColors(true);
    ui->VideofileTableview->setAlternatingRowColors(true);
    ui->VideoresultTableview->setAlternatingRowColors(true);


    //设置每一次选中的是一整行也不是一个单独的item
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_SearchFile->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_SearchResult->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->VideosearchTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->VideofileTableview->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->VideoresultTableview->setSelectionBehavior(QAbstractItemView::SelectRows);

    //表头信息显示居左

    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);
    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);

    ui->tableView_SearchFile->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);
    ui->tableView_SearchFile->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);

    ui->tableView_SearchResult->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);
    ui->tableView_SearchResult->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);

    ui->VideosearchTableView->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);
    ui->VideosearchTableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);

    ui->VideofileTableview->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);
    ui->VideofileTableview->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);

    ui->VideoresultTableview->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);
    ui->VideoresultTableview->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);


    //设置列宽可伸缩

//    ui->tableView->horizontalHeader()->setSectionResizeMode (0,QHeaderView::Stretch);
//    ui->tableView->horizontalHeader()->setSectionResizeMode (1,QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode (0,QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setSectionResizeMode (1,QHeaderView::Interactive);
    ui->tableView->setColumnWidth(0,170);
    ui->tableView->setColumnWidth(1,370);

    ui->VideosearchTableView->horizontalHeader()->setSectionResizeMode (0,QHeaderView::Interactive);
    ui->VideosearchTableView->horizontalHeader()->setSectionResizeMode (1,QHeaderView::Interactive);
    ui->VideosearchTableView->setColumnWidth(0,170);
    ui->VideosearchTableView->setColumnWidth(1,370);

    ui->tableView_SearchFile->horizontalHeader()->setSectionResizeMode (0,QHeaderView::Stretch);
    ui->tableView_SearchFile->horizontalHeader()->setSectionResizeMode (1,QHeaderView::Stretch);
    ui->tableView_SearchFile->setColumnWidth(0,150);
    ui->tableView_SearchFile->setColumnWidth(1,240);


    ui->VideofileTableview->horizontalHeader()->setSectionResizeMode (0,QHeaderView::Interactive);
    ui->VideofileTableview->horizontalHeader()->setSectionResizeMode (1,QHeaderView::Interactive);
    ui->VideofileTableview->setColumnWidth(0,150);
    ui->VideofileTableview->setColumnWidth(1,300);

    ui->tableView_SearchResult->horizontalHeader()->setSectionResizeMode (0,QHeaderView::Interactive);
    ui->tableView_SearchResult->horizontalHeader()->setSectionResizeMode (1,QHeaderView::Interactive);
    ui->tableView_SearchResult->horizontalHeader()->setSectionResizeMode (2,QHeaderView::Interactive);
    ui->tableView_SearchResult->horizontalHeader()->setSectionResizeMode (3,QHeaderView::Interactive);
    ui->tableView_SearchResult->setColumnWidth(0,101);
    ui->tableView_SearchResult->setColumnWidth(1,102);
    ui->tableView_SearchResult->setColumnWidth(2,101);
    ui->tableView_SearchResult->setColumnWidth(3,102);


    ui->VideoresultTableview->horizontalHeader()->setSectionResizeMode (0,QHeaderView::Interactive);
    ui->VideoresultTableview->horizontalHeader()->setSectionResizeMode (1,QHeaderView::Interactive);
    ui->VideoresultTableview->horizontalHeader()->setSectionResizeMode (2,QHeaderView::Interactive);
    ui->VideoresultTableview->setColumnWidth(0,150);
    ui->VideoresultTableview->setColumnWidth(1,120);
    ui->VideoresultTableview->setColumnWidth(2,200);


    //设置选中时为整行选中
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_SearchFile->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_SearchResult->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->VideofileTableview->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->VideosearchTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->VideoresultTableview->setSelectionBehavior(QAbstractItemView::SelectRows);

    //设置表格的单元为只读属性，即不能编辑
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_SearchFile->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_SearchResult->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->VideofileTableview->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->VideosearchTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->VideoresultTableview->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    connect(ui->tableView_SearchResult->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(slotHeaderClicked(int)));//表头排序

    //设置表格的滚动条始终显示
    ui->VideofileTableview->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->VideofileTableview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->VideoresultTableview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->VideoresultTableview->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->tableView_SearchFile->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    //如果你用在QTableView中使用右键菜单，需启用该属性
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView_SearchResult,SIGNAL(clicked(QModelIndex)),this,SLOT(table_view_row_clicked(QModelIndex)));
    connect(ui->tableView_SearchFile,SIGNAL(clicked(QModelIndex)),this,SLOT(tableView_SearchFile_clicked(QModelIndex)));
}


void MainWindow::TestTableSearchResult()
{
    for (int i=0; i< 5; ++i)
    {

        int CurrentRow=table_model_SearchResult->rowCount();
        qDebug()<<"当前行数:"<<CurrentRow;

        table_model_SearchResult->setItem(CurrentRow, 0, new QStandardItem("nicai1"));
        table_model_SearchResult->setItem(CurrentRow, 1, new QStandardItem("sajidaji"));
        table_model_SearchResult->setItem(CurrentRow, 2, new QStandardItem("nicai2"));
        int value=i*5+80;
//        qDebug()<<value;
        table_model_SearchResult->setItem(CurrentRow, 3, new QStandardItem(QString::number(value)));
//         table_model_SearchResult->setItem(CurrentRow, 3, new QStandardItem(value));
        table_model_SearchResult->item(CurrentRow, 0)->setForeground(QBrush(QColor(0, 0, 0)));
        table_model_SearchResult->item(CurrentRow, 1)->setForeground(QBrush(QColor(0, 0, 0)));
        table_model_SearchResult->item(CurrentRow, 2)->setForeground(QBrush(QColor(0, 0, 0)));
        table_model_SearchResult->item(CurrentRow, 3)->setForeground(QBrush(QColor(0, 0, 0)));
    }
}

void MainWindow::initUserTable()
{
    //初始化表头
    //一、添加表头：

    table_model_UserList = new QStandardItemModel();
    table_model_UserList->setColumnCount(4);
    table_model_UserList->setHeaderData(0,Qt::Horizontal,tr("编号"));
    table_model_UserList->setHeaderData(1,Qt::Horizontal,tr("用户名"));
    table_model_UserList->setHeaderData(2,Qt::Horizontal,tr("用户角色"));
    table_model_UserList->setHeaderData(3,Qt::Horizontal,tr("备注"));

    table_model_UserList_tab2 = new QStandardItemModel();
    table_model_UserList_tab2->setColumnCount(4);
    table_model_UserList_tab2->setHeaderData(0,Qt::Horizontal,tr("编号"));
    table_model_UserList_tab2->setHeaderData(1,Qt::Horizontal,tr("用户名"));
    table_model_UserList_tab2->setHeaderData(2,Qt::Horizontal,tr("用户角色"));
    table_model_UserList_tab2->setHeaderData(3,Qt::Horizontal,tr("备注"));
    //二、设置表格属性：
    ui->tableView_UserList->setModel(table_model_UserList);
    ui->tableView_Tab2_UserList->setModel(table_model_UserList);
    //设置表隔行变色
    ui->tableView_UserList->setAlternatingRowColors(true);
    ui->tableView_Tab2_UserList->setAlternatingRowColors(true);
    //设置每一次选中的是一整行也不是一个单独的item
    ui->tableView_UserList->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_Tab2_UserList->setSelectionBehavior(QAbstractItemView::SelectRows);
    //表头信息显示居左
    ui->tableView_UserList->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);
    ui->tableView_UserList->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->tableView_Tab2_UserList->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);
    ui->tableView_Tab2_UserList->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    //设置列宽可伸缩
    ui->tableView_UserList->horizontalHeader()->setSectionResizeMode (0,QHeaderView::Stretch);
    ui->tableView_UserList->horizontalHeader()->setSectionResizeMode (1,QHeaderView::Stretch);
    ui->tableView_UserList->horizontalHeader()->setSectionResizeMode (2,QHeaderView::Stretch);
    ui->tableView_UserList->horizontalHeader()->setSectionResizeMode (3,QHeaderView::Stretch);
    ui->tableView_UserList->setColumnWidth(0,101);
    ui->tableView_UserList->setColumnWidth(1,101);
    ui->tableView_UserList->setColumnWidth(2,102);
    ui->tableView_UserList->setColumnWidth(3,101);

    ui->tableView_Tab2_UserList->horizontalHeader()->setSectionResizeMode (0,QHeaderView::Stretch);
    ui->tableView_Tab2_UserList->horizontalHeader()->setSectionResizeMode (1,QHeaderView::Stretch);
    ui->tableView_Tab2_UserList->horizontalHeader()->setSectionResizeMode (2,QHeaderView::Stretch);
    ui->tableView_Tab2_UserList->horizontalHeader()->setSectionResizeMode (3,QHeaderView::Stretch);
    ui->tableView_Tab2_UserList->setColumnWidth(0,101);
    ui->tableView_Tab2_UserList->setColumnWidth(1,101);
    ui->tableView_Tab2_UserList->setColumnWidth(2,102);
    ui->tableView_Tab2_UserList->setColumnWidth(3,101);
    //设置选中时为整行选中
    ui->tableView_UserList->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_Tab2_UserList->setSelectionBehavior(QAbstractItemView::SelectRows);
    //设置表格的单元为只读属性，即不能编辑
    ui->tableView_UserList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_Tab2_UserList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_Tab2_UserList->setSelectionBehavior(QAbstractItemView::SelectRows);

    //文字居中
//    table_model_UserList->data(Qt::TextAlignmentRole);
//    table_model_UserList_tab2->data(Qt::TextAlignmentRole);
    //如果你用在QTableView中使用右键菜单，需启用该属性
//    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);

    QSqlDatabase db = QSqlDatabase::database("my_sql_connection"); //建立数据库连接
    //第一句话的参数 QT默认的是qt_sql_default_connection  当然你也可以再之前addDatabase的时候第二个参数随便指定 然后这里的参数就是
    //之前的第二个参数的值
    QSqlQuery sqlQuery(db);
    QString sqlstr="select * from User";
    if( sqlQuery.exec(sqlstr) )
    {
       //读取查询到的记录
       while( sqlQuery.next() )
       {
           int CurrentRow=table_model_UserList->rowCount();
           int CurrentRow_tab2=table_model_UserList_tab2->rowCount();
//           qDebug()<<"当前行数:"<<CurrentRow;

           table_model_UserList->setItem(CurrentRow, 0, new QStandardItem(sqlQuery.value(0).toString()));
           table_model_UserList->setItem(CurrentRow, 1, new QStandardItem(sqlQuery.value(1).toString()));
           table_model_UserList->setItem(CurrentRow, 2, new QStandardItem(sqlQuery.value(3).toString()));
           table_model_UserList->setItem(CurrentRow, 3, new QStandardItem(sqlQuery.value(4).toString()));
           table_model_UserList_tab2->setItem(CurrentRow_tab2, 0, new QStandardItem(sqlQuery.value(0).toString()));
           table_model_UserList_tab2->setItem(CurrentRow_tab2, 1, new QStandardItem(sqlQuery.value(1).toString()));
           table_model_UserList_tab2->setItem(CurrentRow_tab2, 2, new QStandardItem(sqlQuery.value(3).toString()));
           table_model_UserList_tab2->setItem(CurrentRow_tab2, 3, new QStandardItem(sqlQuery.value(4).toString()));

       }
    }

}



void MainWindow::on_NewModel_Clicked()
{
    qDebug()<<"新建模型";
    ui->stackedWidget_main->setCurrentIndex(0);
    ui->tabWidget->setCurrentWidget(ui->tab_3);
}

void MainWindow::on_AnimalSreach_Clicked()
{
    qDebug()<<"动物检索";
    ui->stackedWidget_main->setCurrentIndex(2);
    ui->tabWidget_AnimalSearch->setCurrentWidget(ui->tab_6);
}

void MainWindow::on_OptimizeModel_Clicked()
{
    qDebug()<<"优化模型";
    ui->stackedWidget_main->setCurrentIndex(3);
}

void MainWindow::on_UserManage_Clicked()
{
    qDebug()<<"用户管理";
    initUserTable();
    ui->stackedWidget_main->setCurrentIndex(5);
    ui->tabWidget_UserManage->setCurrentWidget(ui->tab);
}

void MainWindow::on_PhotoDeal_Clicked()
{
    qDebug()<<"地图信息";
    ui->stackedWidget_main->setCurrentIndex(4);
    qDebug()<<"执行loadMap";
    ui->mapWidget->setControl(QString::fromUtf8("{8856F961-340A-11D0-A96B-00C04FD705A2}"));
    ui->mapWidget->setProperty("DisplayAlerts",false);  //不显示任何警告信息。
    ui->mapWidget->setProperty("DisplayScrollBars",true); // 显示滚动条
    ui->mapWidget->dynamicCall("Navigate(const QString&)","file:///D:/AnimalSearch/GaodeMapDemo_1_1.html");
}

void MainWindow::on_Start_Clicked()
{
    qDebug()<<"启动主界面";
    ui->stackedWidget_main->setCurrentIndex(1);
}


void MainWindow::showPath(QString path)
{
    // 比较最新的内容和保存的内容找出区别(变化)
    QStringList currEntryList = m_currentContentsMap[path];
    const QDir dir(path);
    QStringList newEntryList = dir.entryList(QDir::NoDotAndDotDot|QDir::AllDirs|QDir::Files, QDir::DirsFirst);

    QSet<QString> newDirSet = QSet<QString>::fromList(newEntryList);
    QSet<QString> currentDirSet = QSet<QString>::fromList(currEntryList);
     // 添加了文件
    QSet<QString> newFiles = newDirSet - currentDirSet;
    QStringList newFile = newFiles.toList();
    // 文件已被移除
    QSet<QString> deletedFiles = currentDirSet - newDirSet;
    QStringList deleteFile = deletedFiles.toList();
    // 更新当前设置
    m_currentContentsMap[path] = newEntryList;

    if (!newFile.isEmpty() && !deleteFile.isEmpty())
    {
        // 文件/目录重命名
        if ((newFile.count() == 1) && (deleteFile.count() == 1))
        {
            qDebug() << QString("File Renamed from %1 to %2").arg(deleteFile.first()).arg(newFile.first());
        }
    }
    else
    {
        // 添加新文件/目录至Dir
        if (!newFile.isEmpty())
        {
            qDebug() << "New Files/Dirs added: " << newFile;
            QString newfilepath=path;
            newfilepath+='/';
            newfilepath.append(newFile.first());
            qDebug()<<"发送新文件地址"<<newfilepath;
            emit sendFilepath(newfilepath);
//            emit sendFilepath(path);
            emit sendCompleteOneOfPic();

//            foreach (QString file, newFile)
//            {
//                // 处理操作每个新文件....
//            }
        }
        if (!deleteFile.isEmpty())
        {
            qDebug() << "Files/Dirs deleted: " << deleteFile;

            foreach(QString file, deleteFile)
            {
                // 处理操作每个被删除的文件....
            }
        }
    }
}

void MainWindow::showResultPic(QString path)
{
    qDebug()<<"图片标签:图片地址"<<path;
    QFileInfo newpicDir(path);
    qDebug()<<"路径是否存在"<<newpicDir.exists();
    sleep(10);
    QPixmap newImg;
    newImg.load(path);
    qDebug()<<"是否读取失败"<<newImg.isNull();
    qDebug()<<"读取图片";
    if(!(newImg.isNull()))
    {
        qDebug()<<"显示结果图片";
        int width1=ui->resultpiclab->width();
        int height1=ui->resultpiclab->height();
        QPixmap fitpixmap=newImg.scaled(width1,height1,Qt::KeepAspectRatio,Qt::SmoothTransformation);
//        qDebug()<<"生成结果fitpixmap";
        ui->resultpiclab->setPixmap(fitpixmap);
//        qDebug()<<"显示结果fitpixmap";
    }
//        newImg.load(path);

}

void MainWindow::showOrigPic(QString path)
{
    if(piclist.count()==0)
            return;

        QImage presentImg;//当前检索图片
        qDebug()<<"显示原始图像piclist"<<piclist[piccnt]<<"cnt"<<piccnt;
        presentImg.load(piclist[piccnt]);
        if(!(presentImg.isNull())){
            QPixmap pixmap1= QPixmap::fromImage(presentImg);
            int width1=ui->origpiclab->width();
            int height1=ui->origpiclab->height();
    //        QPixmap fitpixmap1=pixmap1.scaled(width1,height1,Qt::KeepAspectRatio,Qt::SmoothTransformation);
            QPixmap fitpixmap1=pixmap1.scaled(width1,height1,Qt::KeepAspectRatio, Qt::SmoothTransformation);

            ui->origpiclab->setPixmap(fitpixmap1);
            qDebug()<<"原始图像读取成功";
        }
        else {
            qDebug()<<"原始图像读取失败";
        }

        qDebug()<<"showOrigPic"<<piccnt;
        piccnt++;
}

void MainWindow::showDetctionresult(QString path)
{
    QFile detection_result("E:/detectionresult.txt");//读取存储结果的txt
    QStringList score_list;
    QString score;
    if(detection_result.open(QFile::ReadOnly))
    {
         QTextStream stream(&detection_result);
         while(!stream.atEnd())
             score_list.append(detection_result.readLine());
    }
    int n=score_list.size();
    score=score_list[n-1];
    score_list=score.split(' ');
//    ui->scorelabel->setText(score_list[1]);
    //    ui->classeslabel->setText(score_list[2]);
}

void MainWindow::setmyBarvalue(QString path)
{
    if(piclist.count()==0)
    {
        return;
    }

    qDebug()<<"设置进度条值"<<barcnt;
    ui->progressBar_AnimalSearch->setValue(barcnt);

    barcnt++;
}

//void MainWindow::on_NewModel_1_Clicked()
//{
//    qDebug()<<"新建模型-模型设置";

//}

//void MainWindow::on_NewModel_2_Clicked()
//{
//    qDebug()<<"新建模型-新建模型";

//}

//void MainWindow::on_NewModel_3_Clicked()
//{
//    qDebug()<<"新建模型-训练设置";

//}

//void MainWindow::on_AnimalSreach_1_Clicked()
//{
//    qDebug()<<"动物检索-模型设置";

//}

//void MainWindow::on_AnimalSreach_2_Clicked()
//{
//    qDebug()<<"动物检索-检索界面";

//}

//void MainWindow::on_OptimizeModel_1_Clicked()
//{
//    qDebug()<<"优化模型-模型选择";

//}

//void MainWindow::on_OptimizeModel_2_Clicked()
//{
//    qDebug()<<"优化模型-训练集标注";

//}

//void MainWindow::on_OptimizeModel_3_Clicked()
//{
//    qDebug()<<"优化模型-训练设置";

//}

//void MainWindow::on_UserManage_1_Clicked()
//{
//    qDebug()<<"用户管理-用户查看";

//}

//void MainWindow::on_UserManage_2_Clicked()
//{
//    qDebug()<<"用户管理-用户管理";

//}

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    //如果在treewidget上面点击右键且有焦点就弹出menu
    if(ui->FiletreeView->hasFocus()){
        m_MainMenu->move(cursor().pos());
        m_MainMenu->show();
    }
}

void MainWindow::mkdir()
{
    QModelIndex index = ui->FiletreeView->currentIndex();
    if (!index.isValid()) {
            return;
    }
    QString dirName = QInputDialog::getText(this,tr("Create Directory"),tr("Directory name"));
    if (!dirName.isEmpty()) {
            if (!model->mkdir(index, dirName).isValid()) {
                    QMessageBox::information(this, tr("Create Directory"),  tr("Failed to create the directory"));
            }
    }
}

void MainWindow::rm()
{
    QModelIndex index = ui->FiletreeView->currentIndex();
    if (!index.isValid()) {
            return;
    }
    bool ok;
    if (model->fileInfo(index).isDir()) {
            ok = model->rmdir(index);
    } else {
            ok = model->remove(index);
    }
    if (!ok) {
            QMessageBox::information(this, tr("Remove"),tr("Failed to remove %1").arg(model->fileName(index)));
    }
}

void MainWindow::AddSearch()
{
    //将文件或目录添加进入检索范围
    //如果文件不是图片格式或者文件夹中存在不是图片格式的文件-》提示该文件或文件夹不能加入检索范围
    qDebug()<<"检索";
    QModelIndex index = ui->FiletreeView->currentIndex();
    if (!index.isValid()) {
            return;
    }

    if (model->fileInfo(index).isDir()) {
            //是文件夹
           qDebug()<<"目录地址"<<model->fileInfo(index).filePath();
           //判断这个路径中是不是全部是图片
           QString path=model->fileInfo(index).filePath();
           QDir dir(path);

           /********************************************************/
           QStringList filters;
               filters<<QString("*.jpeg")<<QString("*.jpg")<<QString("*.png")<<QString("*.tiff")<<QString("*.gif")<<QString("*.bmp");
               dir.setFilter(QDir::Files | QDir::NoSymLinks); //设置类型过滤器，只为文件格式
               dir.setNameFilters(filters);  //设置文件名称过滤器，只为filters格式（后缀为.jpeg等图片格式）

               int dir_count = dir.count();
               if(dir_count <= 0)
               {
                   QMessageBox::information(this, tr("Add Search"),tr("该文件夹中不含有图片").arg(model->fileInfo(index).filePath()));
                   return;
               }

               QStringList string_list;
               QStringList string_file_list;
               //获取分隔符
               //QChar separator = QDir::separator();
               QChar separator = QChar('/');
               if(!path.contains(separator))
               {
                   separator = QChar('\\');
               }
               QChar last_char = path.at(path.length()-1);
               if(last_char == separator)
               {
                   separator = QChar();
               }

               foreach(QFileInfo mfi ,dir.entryInfoList())
               {
                   if(mfi.isFile())
                   {
                       QString file_path = path + separator +  mfi.fileName();

                       //把文件加入到string_list中
                       string_list.append(file_path);
                       string_file_list.append(mfi.fileName());
                   }
               }
               QString rowData;
               for(int row=0;row<table_model->rowCount();row++)
               {
                 rowData+=table_model->item(row,1)->text();

               }
               for (int i=0; i< string_list.size(); ++i)
               {
                   if(rowData.contains(string_list.at(i)))//如果检索列表已经包含将要加入的文件
                   {

                        QMessageBox::information(this, tr("Add Search"),tr("%1该图片已经加入检索列列表，不可重复添加").arg(string_list.at(i)));
                   }
                   else {
                       qDebug() << string_list.size();
                       qDebug() << string_list.at(i);
                       int CurrentRow=table_model->rowCount();
                       qDebug()<<"当前行数:"<<CurrentRow;

                       table_model->setItem(CurrentRow, 0, new QStandardItem(string_file_list.at(i)));
                       table_model->setItem(CurrentRow, 1, new QStandardItem(string_list.at(i)));
                       table_model->item(CurrentRow, 0)->setForeground(QBrush(QColor(0, 0, 0)));
                       table_model->item(CurrentRow, 1)->setForeground(QBrush(QColor(0, 0, 0)));
                   }

               }

           /********************************************************/


    }
    else
    {
            //是文件
            qDebug()<<"文件"<<model->fileInfo(index).filePath();
            QString rowData;
            for(int row=0;row<table_model->rowCount();row++)
            {
              rowData+=table_model->item(row,1)->text();

            }
            qDebug()<<"当前所有检索文件"<<rowData;
            if(rowData.contains(model->fileInfo(index).filePath())){
                QMessageBox::information(this, tr("Add Search"),tr("%1该图片已经加入检索列列表,不可重复添加").arg(model->fileInfo(index).filePath()));
            }
            else{
                QPixmap image;
                image.load(model->fileInfo(index).filePath());
                 if(!(image.isNull()))
                 {
                     int CurrentRow=table_model->rowCount();
                     qDebug()<<"当前行数:"<<CurrentRow;

                     table_model->setItem(CurrentRow, 0, new QStandardItem(model->fileInfo(index).fileName()));
                     table_model->setItem(CurrentRow, 1, new QStandardItem(model->fileInfo(index).filePath()));
                     table_model->item(CurrentRow, 0)->setForeground(QBrush(QColor(0, 0, 0)));
                     table_model->item(CurrentRow, 1)->setForeground(QBrush(QColor(0, 0, 0)));

                 }
                 else{

                       QMessageBox::information(this, tr("Add Search"),tr("该文件不是图片格式").arg(model->fileInfo(index).filePath()));

                 }
        }
            }

    ui->picnum_label->setText("当前共有"+QString::number(table_model->rowCount())+"张检索图片");

}

void MainWindow::onSelectLabelText()
{
    qDebug()<<"select";
    QModelIndex index = ui->FiletreeView->currentIndex();
    if (!index.isValid()) {
            return;
    }
    bool ok;
    if (model->fileInfo(index).isDir()) {
//        ui->CurrentDirLabel->setText(model->filePath(index));
//        ui->CurrentFileLabel->setText("未选择文件");
    }
    else
    {
//        ui->CurrentDirLabel->setText(model->filePath(index));
//        ui->CurrentFileLabel->setText(model->fileName(index));
        //qDebug()<<"图片："<<model->filePath(index)+'/'+model->fileName(index);
        QPixmap image;
        image.load(model->filePath(index));
         if(!(image.isNull()))
         {
             qDebug()<<"这是图片文件"<<model->filePath(index);
//             int width2=ui->QImagelabel->width();
//             int height2=ui->QImagelabel->height();
//             ui->QImagelabel->clear();//清空
//             QPixmap fitimage=image.scaled(width2,height2,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
//             image.scaled(ui->QImagelabel->size(), Qt::KeepAspectRatio);
//             ui->QImagelabel->setScaledContents(true);
//             ui->QImagelabel->setPixmap(fitimage);//加载到Label标签
//             ui->QImagelabel->show();//显示

         }
        else{
            qDebug()<<"不是图片"<<model->filePath(index);
        }
//
    }
}

void MainWindow::DelTableRow()
{
    //删除表格中的任意行
//    int CurrentRow=table_model->rowCount();
    int CurrentRow=ui->tableView->currentIndex().row();//当前表格指向哪一行
    qDebug()<<"行数:"<<CurrentRow;
    table_model->removeRow(CurrentRow);
    ui->tableView->setUpdatesEnabled(true);
    ui->picnum_label->setText("当前共有"+QString::number(table_model->rowCount())+"张检索图片");

}




void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    //搜索
    if(arg1.isEmpty()){
        //如果搜索框没有文字 那么就显示全部的表格行
        int CurrentRow=table_model->rowCount();//当前表格一共有多少行
        for(int row = 0 ; row < CurrentRow ; row++){

            ui->tableView->showRow(row);
        }
        return;
    }
    int rowCount = table_model->rowCount();//当前表格一共有多少行
    int ColumnCount = table_model->columnCount();//当前表格一共有多少列
    for(int row = 0 ; row < rowCount ; row++){
        QString rowData;
        for(int column = 0 ; column < ColumnCount ; column++){
            if(table_model->item(row,column)){
                //qDebug()<<"单元格内容:"<<table_model->item(row,column)->text();
                rowData += table_model->item(row,column)->text();
            }
        }
        if(!rowData.isEmpty()){
            //有数据
            if(rowData.contains(arg1)){
                ui->tableView->showRow(row);
            }
            else{
                ui->tableView->hideRow(row);
            }
        }
        else{
            //无数据
            ui->tableView->hideRow(row);
        }
    }



}

void MainWindow::on_StartSearchBtn()
{
    //开始检索
    //首先切换到检索界面
    barcnt=1;
    piccnt=0;
    QFile file("D:\\AnimalSearch\\filePath.txt");
    if (file.exists())
    {
           qDebug()<<"delete";
            file.remove();
    }
    qDebug()<<"开始检索";
    //先判断这个表格里面有没有内容 如果没有内容 说明尚未有图片加入检索 然后就不能进入下一页 应该写一个提示信息（弹出框）
    if(!ui->tableView->verticalHeader()->count()){
        //空表格
        QMessageBox::about(NULL, "警告", "<font color='red'>当前并未有图片加入检索</font>");
        //QMessageBox::information(this, tr("Start Search"),tr("当前并未有图片加入检索").arg(model->Info(index).Path()));

    }
    else{
         ui->StartSearchBtn->setEnabled(false);//点击了开始检索就不能再次点击
         ui->setnewPicSearch->setEnabled(true);//点击了开始检索可以新建检索任务
         ui->tabWidget_AnimalSearch->setCurrentWidget(ui->tab_7);
         //PS 点击开始检索之后应该就需要实时检索了吧 这个时候是不是就要开线程了？
         //把表格导入到下一页的表格中
//         for(int i=0;i<table_model_SearchFile->rowCount();i++){
//             table_model_SearchFile->removeRow(0);
//         }
//         for(int i=0;i<table_model_SearchResult->rowCount();i++){
//             table_model_SearchResult->removeRow(0);
//         }//清空上次检测文件的文件列表


         int rowCount = table_model->rowCount();//当前表格一共有多少行
         // 连接信号槽
         //点击了开始检索之后就应该开启进度条的线程了...

//         connect(ui->StartSearchBtn, SIGNAL(clicked(bool)), this, SLOT(startProgressBarThread()));
//         emit sendValuesOfFiles(rowCount);
         qDebug()<<"&&&&&&&&&&&&&&&&&&";
         int ColumnCount = table_model->columnCount();//当前表格一共有多少列
         for(int row = 0 ; row < rowCount ; row++){
             QString RowColumnData;//表格每一行的所有信息
             QString RowfilePathData;//表格每一行的文件路径信息
             RowfilePathData += table_model->item(row,1)->text()+" ";
             for(int column = 0 ; column < ColumnCount ; column++){
                 if(table_model->item(row,column)){
                     //qDebug()<<"单元格内容:"<<table_model->item(row,column)->text();
                     //一个单元格一个单元格的导入
                     RowColumnData = table_model->item(row,column)->text();


                     table_model_SearchFile->setItem(row, column, new QStandardItem(RowColumnData));
                     table_model_SearchFile->item(row, column)->setForeground(QBrush(QColor(0, 0, 0)));
                     table_model_SearchFile->item(row, column)->setForeground(QBrush(QColor(0, 0, 0)));

                 }
             }
//             qDebug()<<RowfilePathData;
//             WriteThread writethread(RowfilePathData);
//             writethread.start();


             if(!file.open(QIODevice::WriteOnly|QIODevice::Append))
             {
                qDebug()<<"单元格内容:Open failed.";
             }
             else{
                 qDebug()<<"Open"<<RowfilePathData;
                 file.write(RowfilePathData.toUtf8());
                 file.close();
             }

         }

         //开启图片检索
         loadMap();
//         mythread=new DetectionThread(this);//检测线程
         myWatcher=new QFileSystemWatcher;//文件监控器变量
         cmd_mythread=new DetectionThread_cmd(this);

//         if(!mythread->isRunning()){
//            mythread->start();
//            mythread->quit();
//         }
//         else {
//             qDebug()<<"正在执行检测";
//         }

         QFile file_origpic("D:/AnimalSearch/filePath.txt");//保存原始文件的路径
         if(file_origpic.open(QFile::ReadOnly))
         {
             QTextStream stream(&file_origpic);
             //如果未到达文件结尾，将文件名依次读取到picList列表中
             while(!stream.atEnd())
             {
                 QByteArray line=file_origpic.readLine();
                 filename=QString(line);
             }
             piclist=filename.split(' ');
             file_origpic.close();

         }
         else
         {
             //如果打开文件失败，显示错误提示框
             QMessageBox::critical(this, tr("错误"), file_origpic.errorString());
             exit(1);
         }//将文本文件中的内容存到列表中
         qDebug()<<"检测图片张数为"<<piclist.size();
         ui->progressBar_AnimalSearch->setMinimum(0);
         ui->progressBar_AnimalSearch->setMaximum(piclist.size()-1);
         ui->progressBar_AnimalSearch->setValue(0);
         QFileInfo file("D:/AnimalSearch/picresult");
         if (file.isDir())
         {
             const QDir dirw("D:/AnimalSearch/picresult");
             m_currentContentsMap["D:/AnimalSearch/picresult"] = dirw.entryList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files, QDir::DirsFirst);
         //获取监控路径中的所有文件
         }
//         int num=piclist.count()/2;
//         int i=0;
//         for(i=0;i<num;i++)
//         {
//             piclist2.append(piclist[i]);
//             piclist2.append(piclist[i+num]);
//         }
         if(!cmd_mythread->isRunning()){
            cmd_mythread->start();
//             cmd_mythread->quit();
         }
         else {
             qDebug()<<"正在执行检测";
         }
         myWatcher->addPath("D:/AnimalSearch/picresult");
         connect(myWatcher,SIGNAL(directoryChanged(QString)),this,SLOT(showPath(QString)));
         connect(this,SIGNAL(sendFilepath(QString)),this,SLOT(showResultPic(QString)));
         connect(this,SIGNAL(sendFilepath(QString)),this,SLOT(showOrigPic(QString)));
//         connect(this,SIGNAL(sendFilepath(QString)),this,SLOT(showDetctionresult(QString)));
         connect(this,SIGNAL(sendFilepath(QString)),this,SLOT(setmyBarvalue(QString)));
         connect(this,SIGNAL(sendFilepath(QString)),this,SLOT(importCSVFile(QString)));


    }


}

void MainWindow::on_lineEdit_Threshold_textChanged(const QString &arg1)
{
    for(int i=table_model_SearchResult->rowCount()-1;i>-1;--i)
    {
        table_model_SearchResult->removeRow(i);
    }
    //阈值搜索
    qDebug()<<"阈值搜索:"<<arg1;
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

    if(arg1.isEmpty()){
        //如果搜索框没有输入阈值 那么就显示全部的表格行
        int CurrentRow=table_model_SearchResult->rowCount();//当前表格一共有多少行
        for(int row = 0 ; row < CurrentRow ; row++){

            ui->tableView_SearchResult->showRow(row);
        }
        return;
    }
    int rowCount = table_model_SearchResult->rowCount();//当前表格一共有多少行
    for(int row = 0 ; row < rowCount ; row++){
        float ThreSholdValue;
        if(table_model_SearchResult->item(row,2)){
                //qDebug()<<"单元格内容:"<<table_model->item(row,column)->text();
                ThreSholdValue = table_model_SearchResult->item(row,2)->text().toFloat();
            }
            //有数据
            if(ThreSholdValue >= arg1.toFloat()){
                ui->tableView_SearchResult->showRow(row);
            }
            else{
//                ui->tableView_SearchResult->hideRow(row);
                table_model_SearchResult->removeRow(row);
            }


    }

}

void MainWindow::on_pushButton_ResultSort_clicked()
{
    clickFlag++;
    if(clickFlag % 2 ==0){
        //
        ui->pushButton_ResultSort->setText("结果排序↓");
        ui->tableView_SearchResult->setSortingEnabled(true);
         ui->tableView_SearchResult->sortByColumn(2,Qt::DescendingOrder);
//        table_model_SearchResult->sort(3,Qt::DescendingOrder);

    }
    else{
        ui->pushButton_ResultSort->setText("结果排序↑");
//        ui->tableView_SearchResult->setSortingEnabled(true);
        ui->tableView_SearchResult->sortByColumn(2,Qt::AscendingOrder);
//        table_model_SearchResult->sort(3,Qt::AscendingOrder);
    }
}

void MainWindow::on_NewModel_NextBtn1_Clicked()
{
    //新建模型-模型设置-下一步按钮槽函数
    //跳转到下一页
    QProcess::startDetached("D:/AnimalSearch/labelImg/labelImg.exe");
    Sleep(1500);
//    ui->tabWidget_NewModel->setCurrentWidget(ui->tab_4);
}

void MainWindow::handleResults(int value)
{
    //更新进度条
    qDebug()<<"Handle Thread:"<< QThread::currentThreadId();
    ui->progressBar_AnimalSearch->setValue(value);
}

void MainWindow::startProgressBarThread()
{
    qDebug()<<"开启进度条线程";
    //开启进度条线程
//    ProgressBarThread *progressBarThread = new ProgressBarThread(this);
//    progressBarThread->start();
//    m_progressBarThread->start();
//    connect(m_progressBarThread,SIGNAL(resultReady(int)),this,SLOT(handleResults(int)));
//    //告知有多少个检索文件数
//    connect(this, SIGNAL(sendValuesOfFiles(int)), progressBarThread, SLOT(recieveValuesOfFiles(int)));
    //线程结束后自动销毁
//    connect(m_progressBarThread,SIGNAL(finished()), m_progressBarThread, SLOT(deleteLater()));

}


void MainWindow::on_DelUserBtn_clicked()
{
    //删除用户
    qDebug()<<"删除用户";
    int CurrentRow=ui->tableView_Tab2_UserList->currentIndex().row();//当前表格指向哪一行
    qDebug()<<"行数:"<<CurrentRow;
    QString id = table_model_UserList_tab2->item(CurrentRow,0)->text();
//    table_model->removeRow(CurrentRow);
    //同时删除数据库中的对应行

    QSqlDatabase db = QSqlDatabase::database("my_sql_connection"); //建立数据库连接
    //第一句话的参数 QT默认的是qt_sql_default_connection  当然你也可以再之前addDatabase的时候第二个参数随便指定 然后这里的参数就是
    //之前的第二个参数的值
    QSqlQuery sqlQuery(db);
    QString sqlstr="Delete from User where id="+id;
    if( sqlQuery.exec(sqlstr) ){
        qDebug()<<"删除成功";
    }
    else{
        qDebug()<<"删除失败";
    }
    table_model_UserList_tab2->removeRow(CurrentRow);
    table_model_UserList->removeRow(CurrentRow);

}

void MainWindow::on_AddUserBtn_clicked()
{
    //添加用户
    //添加新用户
    qDebug()<<"添加新用户";
    dialog->show();

}


void MainWindow::on_UpdateUserBtn_clicked()
{
    //更新用户
//    QMessageBox msgBox;
//    msgBox.setWindowTitle("提示");
//    msgBox.setText("更新用户功能暂时没做");
//    msgBox.exec();

 int CurrentRow=ui->tableView_Tab2_UserList->currentIndex().row();//当前表格指向哪一行
 qDebug()<<CurrentRow;
 if(CurrentRow == -1){
         QMessageBox msgBox;
         msgBox.setWindowTitle("提示");
         msgBox.setText("您尚未选择哪位用户进行编辑，请点击想要编辑的用户行");
         msgBox.exec();
         return;
 }
 QString id = table_model_UserList_tab2->item(CurrentRow,0)->text();
 QSqlDatabase db = QSqlDatabase::database("my_sql_connection"); //建立数据库连接
 QSqlQuery sqlQuery(db);
 QString sqlstr="Select * from User where id="+id;
 qDebug()<<sqlstr;
 if( sqlQuery.exec(sqlstr) )
 {
        qDebug()<<"数据获取成功";
        sqlQuery.first();
        int id = sqlQuery.value(0).toString().toInt();
        QString username = sqlQuery.value(1).toString();
        QString userpwd = sqlQuery.value(2).toString();
        QString userrole = sqlQuery.value(3).toString();
        QString description = sqlQuery.value(4).toString();

        emit SendSignalToUpdateUserDialog(id,username,userpwd,userrole,description);
        updateDialog->show();//这句话一定要写在emit的后面,不然你发送信号之前就打开dialog了 传过来的信号就无法显示在上面了。

 }
 else
 {
     qDebug()<<"数据获取失败";
 }

}

void MainWindow::table_view_row_clicked(const QModelIndex &index)
{
    QAbstractItemModel  *modessl = ui->tableView_SearchResult->model();
    int curRow  = index.row();
    //结果图片地址
    QModelIndex indextemp01 = modessl->index(curRow,1);//遍历第一行的所有列
    //这个是一个单元格的值。tostring()----ok
    QString datatemp01 = modessl->data(indextemp01).toString();
    CurrentPath = datatemp01;
    datatemp01.replace('/',QChar('\\'));
//        datatemp01.replace('\\',QRegExp("\\/"));
    qDebug()<<"datatemp01:"<<datatemp01;
    QPixmap image;
    image.load(datatemp01);
     if(!(image.isNull()))
     {
         qDebug()<<"这是图片文件"<<datatemp01;
         int width2=ui->QImagelabel_2->width();
         int height2=ui->QImagelabel_2->height();
         QPixmap fitpixmap=image.scaled(width2,height2,Qt::KeepAspectRatio, Qt::SmoothTransformation);
         ui->QImagelabel_2->setPixmap(fitpixmap);
//         ui->QImagelabel_2->clear();//清空
//         image.scaled(ui->QImagelabel_2->size(), Qt::KeepAspectRatio);
//         ui->QImagelabel_2->setScaledContents(true);
//         ui->QImagelabel_2->setPixmap(image);//加载到Label标签
//         ui->QImagelabel_2->show();//显示

     }
     else{
         qDebug()<<"这不是图片文件"<<datatemp01;
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
    ui->lineEdit_jing->setText(datatemp02);
    ui->lineEdit_wei->setText(datatemp03);

    QAxObject *document = ui->webWidget->querySubObject("Document");
    QAxObject *parentWindow = document->querySubObject("parentWindow");

    parentWindow->dynamicCall("getPicPath(const QString&,const QString&,const QString&)",datatemp02,datatemp03,datatemp01); //调用参数

    //    parentWindow->dynamicCall("execScript(QString,QString)",QString("qtLocation(%1,%2)").arg(datatemp02).arg(datatemp03),"JavaScript");
}

void MainWindow::tableview_picfile_clicked(const QModelIndex &index)
{
    QAbstractItemModel *model2=ui->tableView->model();
    int curRow  = index.row();
    QModelIndex indextemp2=model2->index(curRow,1);
    QString datatemp2=model2->data(indextemp2).toString();
    qDebug()<<"点击预览文件"<<datatemp2;
    QImage picfile;
    picfile.load(datatemp2);
    QPixmap pixmap2=QPixmap::fromImage(picfile);
    int width2=ui->piclabel->width();
    int height2=ui->piclabel->height();
    QPixmap fitpixmap2=pixmap2.scaled(width2,height2,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui->piclabel->setPixmap(fitpixmap2);
}

void MainWindow::importCSVFile(QString path)
{
    qDebug()<<"执行import";
    QString fileName = "D:\\AnimalSearch\\presentresult.csv" ;
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
        qDebug()<<"当前行数:"<<CurrentRow<<"当前内容:"<<list.at(0);
        double threashold = 85;
        QString number(list.at(2));
        double result_number = number.toDouble();

        qDebug()<<"可能性:"<<result_number;
        if(result_number >= threashold){
            emit SendSignalToBigMapDialog(list.at(4),list.at(5),list.at(6));
            qDebug()<<"加入big11111Map";
        }
        else {
            qDebug()<<"不加入bigMap";
        }
            for(int item = 0; item < 7; item++)
            {
                    //QString number = list.at(2);
//                    double threashold = 98;
//                    QString number(list.at(2));
//                    double result_number = number.toDouble();

//                    qDebug()<<"可能性:"<<result_number;
//                    if(result_number >= threashold){
//                        emit SendSignalToBigMapDialog(list.at(4),list.at(5),list.at(6));
//                        qDebug()<<"加入big11111Map";
//                    }
//                    else {
//                        qDebug()<<"不加入bigMap";
//                    }
//                if(item==2)
//                {
//                    QString result=list.at(item);
//                    result=float(result);
//                    table_model_SearchResult->setItem(CurrentRow, item, new QStandardItem(result));
//                    table_model_SearchResult->item(CurrentRow, item)->setForeground(QBrush(QColor(0, 0, 0)));
//                }

                    table_model_SearchResult->setItem(CurrentRow, item, new QStandardItem(list.at(item)));
                    table_model_SearchResult->item(CurrentRow, item)->setForeground(QBrush(QColor(0, 0, 0)));

            }

            i++;
        }

     file.close();
}

void MainWindow::on_ShowDataBase()
{
  dialog->close();

  qDebug() <<"发射接受成功！";

  updateUserTable();

}

void MainWindow::updateUserTable(){
    //要先清空table表格再显示！！！！！！！！！！！
//    int allRow=table_model_UserList->rowCount();
//    int allRow1=table_model_UserList_tab2->rowCount();
    table_model_UserList->removeRows(0,table_model_UserList->rowCount());
    table_model_UserList_tab2->removeRows(0,table_model_UserList_tab2->rowCount());
//    for(int i=0;i<allRow;i++){
//        table_model_UserList->removeRow(i);
//    }
//    for(int i=0;i<allRow1;i++){
//        table_model_UserList_tab2->removeRow(i);
//    }
    QSqlDatabase db1 = QSqlDatabase::database("my_sql_connection"); //建立数据库连接
    //第一句话的参数 QT默认的是qt_sql_default_connection  当然你也可以再之前addDatabase的时候第二个参数随便指定 然后这里的参数就是
    //之前的第二个参数的值
    QSqlQuery sqlQuery(db1);
    QString sqlstr1="select * from User";
    if( sqlQuery.exec(sqlstr1) )
    {
       //读取查询到的记录
       while( sqlQuery.next() )
       {
           int CurrentRow=table_model_UserList->rowCount();
           int CurrentRow_tab2=table_model_UserList_tab2->rowCount();
  //         dqDebug()<<"当前行数:"<<CurrentRow;

           table_model_UserList->setItem(CurrentRow, 0, new QStandardItem(sqlQuery.value(0).toString()));
           table_model_UserList->setItem(CurrentRow, 1, new QStandardItem(sqlQuery.value(1).toString()));
           table_model_UserList->setItem(CurrentRow, 2, new QStandardItem(sqlQuery.value(3).toString()));
           table_model_UserList->setItem(CurrentRow, 3, new QStandardItem(sqlQuery.value(4).toString()));

           table_model_UserList_tab2->setItem(CurrentRow_tab2, 0, new QStandardItem(sqlQuery.value(0).toString()));
           table_model_UserList_tab2->setItem(CurrentRow_tab2, 1, new QStandardItem(sqlQuery.value(1).toString()));
           table_model_UserList_tab2->setItem(CurrentRow_tab2, 2, new QStandardItem(sqlQuery.value(3).toString()));
           table_model_UserList_tab2->setItem(CurrentRow_tab2, 3, new QStandardItem(sqlQuery.value(4).toString()));

       }
    }
}

void MainWindow::loadMap()
{
    qDebug()<<"执行loadMap";
    ui->webWidget->setControl(QString::fromUtf8("{8856F961-340A-11D0-A96B-00C04FD705A2}"));
    ui->webWidget->setProperty("DisplayAlerts",false);  //不显示任何警告信息。
    ui->webWidget->setProperty("DisplayScrollBars",false); // 显示滚动条
    ui->webWidget->dynamicCall("Navigate(const QString&)","file:///D:/AnimalSearch/dingwei_v3.1.html");
}

void MainWindow::sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() <dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

}

void MainWindow::initVideoTreeView()
{
    videomodel = new QFileSystemModel;
    videomodel->setReadOnly(false);            //设置可以修改
    videomodel->setRootPath(QDir::currentPath());

    QStringList filter;
    filter<<"*.mp4"<<".avi"<<"*.wmv"<<"*.flv";
    videomodel->setNameFilters(filter);
//    model->setFilter(QDir::Files);
    videomodel->setNameFilterDisables(false);
    ui->VideotreeView->setModel(videomodel);

    ui->VideotreeView->header()->setStretchLastSection(true);
    ui->VideotreeView->header()->setSortIndicator(0, Qt::AscendingOrder);
    ui->VideotreeView->header()->setSortIndicatorShown(true);
    //treeView->header()->s(true);

    QModelIndex index = videomodel->index(QDir::currentPath());
    ui->VideotreeView->expand(index);      //当前项展开
    ui->VideotreeView->scrollTo(index);    //定位到当前项
    ui->VideotreeView->resizeColumnToContents(0);

    connect(ui->addVideobtn,SIGNAL(clicked()),this,SLOT(addVideoSearch()));
}

void MainWindow::SetBigMap()
{
    qDebug()<<"fangda";
    ui->stackedWidget_main->setCurrentIndex(6);//将页面切换到大地图页面
    emit fangda();
    QString jingValue = ui->lineEdit_jing->text();
    QString weiValue  = ui->lineEdit_wei->text();
//    if(CurrentPath == NULL){
//        QMessageBox msgBox;
//        msgBox.setWindowTitle("提示");
//        msgBox.setText("请点需要显示的图片");
//        msgBox.exec();
//    }
    emit SendSignalToBigMapDialog(jingValue,weiValue,CurrentPath);
//    bigMapDialog->show();//这句话一定要写在emit的后面,不然你发送信号之前就打开dialog了 传过来的信号就无法显示在上面了。
    ui->tabWidget_AnimalSearch->setCurrentWidget(ui->tab_5);
}

void MainWindow::CopyFileBtn()
{
    //复制拷贝文件夹
//    CreateFile("E:\\","DIYFile");
    QString Select_file_path = QFileDialog::getExistingDirectory(this,"请选择或新建文件夹来保存路径","./");
    if(Select_file_path.isEmpty())
    {
        return;
    }
    else
    {
        qDebug() <<"选择的文件夹:"<<Select_file_path << endl;
    }
    int rowCount = table_model_SearchResult->rowCount();//当前表格一共有多少行
    qDebug()<<"表格行数:"<<rowCount;
    QString RowColumnData = NULL;
    QString CurrentData = NULL;
    QString CSVFileName01 = "D:\\AnimalSearch\\TableCSV.csv";
    QFile csvFile01(CSVFileName01);
    if (csvFile01.exists())
    {
        csvFile01.remove();
    }
    QString CSVFileName = "D:\\AnimalSearch\\TableCSV.csv";
    QFile csvFile(CSVFileName);
    if(!csvFile.open(QIODevice::WriteOnly | QIODevice::Append| QIODevice::Text))
    {
         qDebug()<< "Cannot open file for writing: ";
         return;
    }
    for(int i=0;i<rowCount;i++){

         RowColumnData = table_model_SearchResult->item(i,6)->text();
         CurrentData = RowColumnData.replace('/',QChar('\\'));
//         qDebug()<<"第2列第"<<i<<"行表格数据"<<CurrentData;
         csvFile.write(CurrentData.toLocal8Bit()+'\n');
    }
    csvFile.close();
    //开启进度条线程
    CopyFileThread *copyfileThread = new CopyFileThread(this);
    connect(this,SIGNAL(SendCopyFileName(QString)),copyfileThread,SLOT(RecevieCopyFileName(QString)));
    emit SendCopyFileName(Select_file_path);

    connect(copyfileThread,SIGNAL(CopySuccess()),this,SLOT(CopySuccessFunction()));
    copyfileThread->start();
}

void MainWindow::CopySuccessFunction()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("提示");
    msgBox.setText("拷贝完毕！");
    msgBox.exec();
}

void MainWindow::tableView_SearchFile_clicked(const QModelIndex &index)
{
    QAbstractItemModel  *modessl = ui->tableView_SearchFile->model();
    int curRow  = index.row();
    //结果图片地址
    QModelIndex indextemp01 = modessl->index(curRow,1);//获取当前行的第二列内容
    QString datatemp01 = modessl->data(indextemp01).toString();
    CurrentPath = datatemp01;
    datatemp01.replace('/',QChar('\\'));
//        datatemp01.replace('\\',QRegExp("\\/"));
    qDebug()<<"测试点击检索文件列表"<<datatemp01;

      QImage searchimage;//当前检索图片
      searchimage.load(datatemp01);
      QPixmap pixmap1= QPixmap::fromImage(searchimage);
      int width1=ui->origpiclab->width();
      int height1=ui->origpiclab->height();
//        QPixmap fitpixmap1=pixmap1.scaled(width1,height1,Qt::KeepAspectRatio,Qt::SmoothTransformation);
      QPixmap fitpixmap1=pixmap1.scaled(width1,height1,Qt::KeepAspectRatio, Qt::SmoothTransformation);

      ui->origpiclab->setPixmap(fitpixmap1);

      QModelIndex indextemp02=modessl->index(curRow,0);//获取当前行第一列内容
      QString datatemp02=modessl->data(indextemp02).toString();
      datatemp02="D:\\AnimalSearch\\picresult\\result-"+datatemp02;
      qDebug()<<"测试点击检索文件列表，文件名"<<datatemp02;
      QImage resultimage;//检索结果图片
      resultimage.load(datatemp02);
      if(!(resultimage.isNull())){
          QPixmap pixmap2=QPixmap::fromImage(resultimage);
          int width2=ui->resultpiclab->width();
          int height2=ui->resultpiclab->height();
          QPixmap fitpixmap2=pixmap2.scaled(width2,height2,Qt::KeepAspectRatio,Qt::SmoothTransformation);
          ui->resultpiclab->setPixmap(fitpixmap2);
      }
      else {
          qDebug()<<"当前图片无检测结果";
      }

}

void MainWindow::addVideoSearch()
{
    qDebug()<<"视频检索";
    QModelIndex index=ui->VideotreeView->currentIndex();
    if(!index.isValid()){
        return;
    }
    if(model->fileInfo(index).isDir()){
        //是文件夹
       qDebug()<<"目录地址"<<model->fileInfo(index).filePath();
       //判断这个路径中是不是全部视频
       QString path=model->fileInfo(index).filePath();
       QDir dir(path);

       /********************************************************/
       QStringList filters;
           filters<<QString("*.mp4")<<QString("*.flv")<<QString("*.avi");
           dir.setFilter(QDir::Files | QDir::NoSymLinks); //设置类型过滤器，只为文件格式
           dir.setNameFilters(filters);  //设置文件名称过滤器，只为filters格式（后缀为.mp4等视频格式）

           int dir_count = dir.count();
           if(dir_count <= 0)
           {
               QMessageBox::information(this, tr("Add Search"),tr("该文件夹中不含有视频").arg(model->fileInfo(index).filePath()));
               return;
           }

           QStringList string_list;
           QStringList string_file_list;
           //获取分隔符
           //QChar separator = QDir::separator();
           QChar separator = QChar('/');
           if(!path.contains(separator))
           {
               separator = QChar('\\');
           }
           QChar last_char = path.at(path.length()-1);
           if(last_char == separator)
           {
               separator = QChar();
           }

           foreach(QFileInfo mfi ,dir.entryInfoList())
           {
               if(mfi.isFile())
               {
                   QString file_path = path + separator +  mfi.fileName();

                   //把文件加入到string_list中
                   string_list.append(file_path);
                   string_file_list.append(mfi.fileName());
               }
           }

           for (int i=0; i< string_list.size(); ++i)
           {
               qDebug() << string_list.size();
               qDebug() << string_list.at(i);
               int CurrentRow=table_model_videoSearch->rowCount();
               qDebug()<<"当前行数:"<<CurrentRow;

               table_model_videoSearch->setItem(CurrentRow, 0, new QStandardItem(string_file_list.at(i)));
               table_model_videoSearch->setItem(CurrentRow, 1, new QStandardItem(string_list.at(i)));
               table_model_videoSearch->item(CurrentRow, 0)->setForeground(QBrush(QColor(0, 0, 0)));
               table_model_videoSearch->item(CurrentRow, 1)->setForeground(QBrush(QColor(0, 0, 0)));
           }
    }

    else
    {
            //是文件
            qDebug()<<"文件"<<model->fileInfo(index).filePath();

             int CurrentRow=table_model_videoSearch->rowCount();
             qDebug()<<"当前行数:"<<CurrentRow;

             table_model_videoSearch->setItem(CurrentRow, 0, new QStandardItem(model->fileInfo(index).fileName()));
             table_model_videoSearch->setItem(CurrentRow, 1, new QStandardItem(model->fileInfo(index).filePath()));
             table_model_videoSearch->item(CurrentRow, 0)->setForeground(QBrush(QColor(0, 0, 0)));
             table_model_videoSearch->item(CurrentRow, 1)->setForeground(QBrush(QColor(0, 0, 0)));
    }
    ui->videonum_label->setText("当前共有"+QString::number(table_model_videoSearch->rowCount())+"个检索视频");
}

void MainWindow::on_StartVideoSearch()
{
    this->disconnect(SIGNAL(sendFilepath(QString)));
    resultplayer=new QMediaPlayer;
    resultplayer->setVideoOutput(resultVidoWidget);
    resultplayer->setNotifyInterval(200);

    ui->resultvideoend_btn->setEnabled(false);
    ui->resultvideopause_btn->setEnabled(false);
    ui->resultslider->setEnabled(false);
    connect(resultplayer,SIGNAL(positionChanged(qint64)),this,SLOT(onresultPositionChanged(qint64)));
    connect(resultplayer,SIGNAL(durationChanged(qint64)),this,SLOT(onresultDurationChanged(qint64)));

    QFile jindu("D:\\AnimalSearch\\jindu.txt");
    if(jindu.exists())
    {
        jindu.remove();
    }
    QFile file("D:\\AnimalSearch\\videoPath.txt");
    if (file.exists())
    {
           qDebug()<<"delete";
            file.remove();
    }
        qDebug()<<"开始检索";
    if(!ui->VideosearchTableView->verticalHeader()->count()){
        //空表格
        QMessageBox::about(NULL, "警告", "<font color='red'>当前并未有视频加入检索</font>");
        //QMessageBox::information(this, tr("Start Search"),tr("当前并未有图片加入检索").arg(model->Info(index).Path()));

    }
    else {
        ui->tabWidget->setCurrentWidget(ui->tab_4);//将界面转到videosearch界面
        //不能直接清空table_model,不然初始化的表头内容也会被一并清空
        int videofilecount=table_model_videoFile->rowCount();
        for (int row=0; row<videofilecount;row++){
            table_model_videoFile->removeRow(0);
        }//清空上次检测文件的文件列表
        int videoresultcount=table_model_resultVideoFile->rowCount();
        for (int row=0; row<videoresultcount;row++){
            table_model_resultVideoFile->removeRow(0);
        }//清空上次检测的检测结果列表
        int rowCount = table_model_videoSearch->rowCount();//视频检索文件表格一共有多少行
        qDebug()<<"开始视频检索";
        int ColumnCount = table_model_videoSearch->columnCount();//视频检索文件表格一共有多少列
        for(int row = 0 ; row < rowCount ; row++){
            QString RowColumnData;//表格每一行的所有信息
            QString RowfilePathData;//表格每一行的文件路径信息
            RowfilePathData += table_model_videoSearch->item(row,1)->text()+" ";
            for(int column = 0 ; column < ColumnCount ; column++){
                if(table_model_videoSearch->item(row,column)){
                    //qDebug()<<"单元格内容:"<<table_model->item(row,column)->text();
                    //一个单元格一个单元格的导入
                    RowColumnData = table_model_videoSearch->item(row,column)->text();
                    table_model_videoFile->setItem(row, column, new QStandardItem(RowColumnData));
                    table_model_videoFile->item(row, column)->setForeground(QBrush(QColor(0, 0, 0)));
                    table_model_videoFile->item(row, column)->setForeground(QBrush(QColor(0, 0, 0)));
                }
            }

            if(!file.open(QIODevice::WriteOnly|QIODevice::Append))
            {
               qDebug()<<"单元格内容:Open failed.";
            }
            else{
                qDebug()<<"Open"<<RowfilePathData;
                file.write(RowfilePathData.toUtf8());
                file.close();
            }
        }
        //开启视频检索
//        if(videostartFlag==0)
        videoWatcher=new QFileSystemWatcher;//文件监控器变量
        Videodetectionthread_cmd *cmd_videothread=new Videodetectionthread_cmd(this);
//        videothread=new VideodetectThread(this);

//        if(!videothread->isRunning()){
//            videothread->start();
//        }
//        else {
//            qDebug()<<"正在执行检测";
//        }
        if(!cmd_videothread->isRunning()){
            cmd_videothread->start();

        }
        else {
            qDebug()<<"正在执行检测";
        }

        QFileInfo file("D:/AnimalSearch/Videoresult");
        if (file.isDir())
        {
            const QDir dirw("D:/AnimalSearch/Videoresult");
            video_currentContentsMap["D:/AnimalSearch/Videoresult"]=dirw.entryList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files, QDir::DirsFirst);
        }
        videoWatcher->addPath("D:/AnimalSearch/Videoresult");
        connect(videoWatcher,SIGNAL(directoryChanged(QString)),SLOT(showPath(QString)));
        connect(this,SIGNAL(sendFilepath(QString)),this,SLOT(importpresntvideoResult(QString)));

        videoprogressTimer=new QTimer();
        qDebug()<<"创建检索进度条计时器";
        videoprogressTimer->setInterval(800);
        videoprogressTimer->start();
        qDebug()<<"检索开始计时";
        connect(videoprogressTimer,SIGNAL(timeout()),this,SLOT(setVideoProgressbar()));
        //当点击了开始检索后，就在原始视频播放器上播放视频
        QAbstractItemModel *mod=ui->VideosearchTableView->model();
        QModelIndex indextemp=mod->index(0,1);
        QString datatemp=mod->data(indextemp).toString();
        origplayer->setMedia(QUrl::fromLocalFile(datatemp));
        origplayer->setMuted(true);
        origVideoWidget->show();
        origplayer->play();
        ui->origvideopause_btn->setEnabled(true);
        ui->origvideoend_btn->setEnabled(true);
        ui->origslider->setEnabled(true);
        videostartFlag=1;
        ui->setnewVideoSearchbtn->setEnabled(true);//当开启一次检索后，就可以新建检索任务了
        ui->startVideoSearch->setEnabled(false);//此时不能再次点击开始检索

    }
}

void MainWindow::tableview_Videofile_clicked(const QModelIndex &index)
{
    QAbstractItemModel *modessl=ui->VideofileTableview->model();
    int curRow=index.row();
    QModelIndex indextemp01=modessl->index(curRow,1);
    QString datatemp01=modessl->data(indextemp01).toString();
    qDebug()<<datatemp01;
//    if(if_origreload)
//        delete origvideoTimer;
//    if_origreload=true;

    origplayer->setMedia(QUrl::fromLocalFile(datatemp01));
    origplayer->setMuted(false);
    origVideoWidget->show();
    origplayer->play();

    ui->origvideopause_btn->setEnabled(true);
    ui->origvideoend_btn->setEnabled(true);
    ui->origslider->setEnabled(true);
//    ui->origslider->setRange(0,maxValue);
//    qDebug()<<"设置视频进度条最大值";
//    origvideoTimer=new QTimer();
//    qDebug()<<"创建视频计时器";
//    origvideoTimer->setInterval(500);
//    origvideoTimer->start();
    qDebug()<<"开始计时";
//    connect(origvideoTimer,SIGNAL(timeout()),this,SLOT(onorigTimerout()));

}

void MainWindow::tableview_resultVideofile_clicked(const QModelIndex &index)
{
    QAbstractItemModel *modessl=ui->VideoresultTableview->model();
    int curRow=index.row();
    QModelIndex indextemp01=modessl->index(curRow,2);
    QString datatemp01=modessl->data(indextemp01).toString();
    qDebug()<<datatemp01;

//    if(if_resultreload)
//        delete resultvideoTimer;
//    if_resultreload=true;

    resultplayer->setMedia(QUrl::fromLocalFile(datatemp01));
    resultplayer->setMuted(true);
    resultVidoWidget->show();
    resultplayer->play();

    ui->resultvideopause_btn->setEnabled(true);
    ui->resultvideoend_btn->setEnabled(true);
    ui->resultslider->setEnabled(true);
    ui->resultslider->setRange(0,maxValue);
//    resultvideoTimer=new QTimer;
//    resultvideoTimer->setInterval(500);
//    resultvideoTimer->start();
//    connect(resultvideoTimer,SIGNAL(timeout()),this,SLOT(onresultTimerout()));


}


void MainWindow::on_origvideopause_btn_clicked()
{
    static bool flag=true;
    if(flag){
        origplayer->pause();
        ui->origvideopause_btn->setText("播放");
    }
    else {
        origplayer->play();
        ui->origvideopause_btn->setText("暂停");
    }
    flag=!flag;
}

void MainWindow::on_origvideoend_btn_clicked()
{
    origplayer->stop();
}

void MainWindow::on_resultvideopause_btn_clicked()
{
    static bool flag=true;
    if(flag){
        resultplayer->pause();
        ui->resultvideopause_btn->setText("播放");
    }
    else {
        resultplayer->play();
        ui->resultvideopause_btn->setText("暂停");
    }
    flag=!flag;
}

void MainWindow::on_resultvideoend_btn_clicked()
{
    resultplayer->stop();
}

void MainWindow::importpresntvideoResult(QString str)
{

    QString fileName="D:/AnimalSearch/presentvideoresult.csv";
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
       qDebug()<<"未能导入presentvideoresult.csv文件 ";
       return;
    }
    qDebug()<<"导入presentvideoresult";
    QStringList list;
    list.clear();
    QTextStream in(&file);
//    int i =0;
    while(!in.atEnd())
    {
        QString fileLine=in.readLine();
        list=fileLine.split(",",QString::SkipEmptyParts);
        int CurrentRow=table_model_resultVideoFile->rowCount();
        qDebug()<<"当前行数:"<<CurrentRow<<"当前内容"<<list.at(2);
        for(int item=0;item<3;item++)
        {
            table_model_resultVideoFile->setItem(CurrentRow,item,new QStandardItem(list.at(item)));
            table_model_resultVideoFile->item(CurrentRow,item)->setForeground(QBrush(QColor(0,0,0)));
        }

    }
    file.close();
}



void MainWindow::on_DelVideoFile_clicked()
{
    int CurrentRow=ui->VideosearchTableView->currentIndex().row();
    qDebug()<<"行数"<<CurrentRow;
    table_model_videoSearch->removeRow(CurrentRow);
    ui->videonum_label->setText("当前共有"+QString::number(table_model_videoSearch->rowCount())+"个检索视频");
}

void MainWindow::onorigTimerout()
{
//    qDebug()<<"origTimerout函数";
    int position=origplayer->position()*maxValue/origplayer->duration();
//    qDebug()<<origplayer->position()*maxValue/origplayer->duration();
//    ui->origslider->setValue(origplayer->position()*maxValue/origplayer->duration());
    ui->origslider->setValue(position);

}

void MainWindow::onresultTimerout()
{
    int position=resultplayer->position()*maxValue/resultplayer->duration();
//    ui->resultslider->setValue(resultplayer->position()*maxValue/resultplayer->duration());
    ui->resultslider->setValue(position);
}

void MainWindow::origslider_clicked()
{
    origplayer->setPosition(ui->origslider->value()*origplayer->duration()/maxValue);
}

void MainWindow::resultslider_clicked()
{
    resultplayer->setPosition(ui->resultslider->value()*resultplayer->duration()/maxValue);
}

void MainWindow::origslider_moved()
{
//    origvideoTimer->stop();
    origplayer->setPosition(ui->origslider->value()*origplayer->duration()/maxValue);
}

void MainWindow::resultslider_moved()
{
//    resultvideoTimer->stop();
    resultplayer->setPosition(ui->resultslider->value()*resultplayer->duration()/maxValue);
}

void MainWindow::origslider_released()
{
//    origvideoTimer->start();
}

void MainWindow::resultslider_released()
{
//    resultvideoTimer->start();
}

void MainWindow::setVideoProgressbar()
{
    qDebug()<<"设置视频检索进度条";
    QString fileName="D:/AnimalSearch/jindu.txt";
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))

    {
       qDebug()<<"Cannot open jindu.txt: ";
    }
    else
    {
        QStringList list;
        list.clear();
        QTextStream in(&file);
        while(!in.atEnd())
        {
            QString fileLine = in.readLine();
            list = fileLine.split(" ", QString::SkipEmptyParts);
        }
        qDebug()<<"当前检测"<<list[0];
        qDebug()<<"总帧数"<<list[1];
        QString num1=list[0];
        QString num2=list[1];
        QString count=list[2];
        QString nums=list[3];
        double position1=num1.toDouble();
        double position2=num2.toDouble();
        double value=(position1/position2)*110;
        if(value>97)
        {
            value=100;

        }
        qDebug()<<value;
        ui->progressBar->setValue(value);
        ui->label_count->setText(count+"/"+nums);

    }





}

void MainWindow::initMap()
{

    big_WebView=new QWebEngineView;
    big_WebView->load(QUrl::fromLocalFile("D:/AnimalSearch/GaodeMapDemo.html"));
    big_WebView->showMaximized();
    QGridLayout *bigMapLayout=new QGridLayout(ui->tab_5);
    bigMapLayout->addWidget(ui->bigmap_table,0,0);
    bigMapLayout->addWidget(big_WebView,0,1);
}

void MainWindow::init_Bigmaptable()
{
    table_model_bigmap=new QStandardItemModel();
    //init table view

    table_model_bigmap->setColumnCount(6);
    table_model_bigmap->setHeaderData(0,Qt::Horizontal,tr("检索成功文件名"));
    table_model_bigmap->setHeaderData(1,Qt::Horizontal,tr("所处文件夹位置"));
    table_model_bigmap->setHeaderData(2,Qt::Horizontal,tr("识别可能性"));
    table_model_bigmap->setHeaderData(3,Qt::Horizontal,tr("识别种类"));
    table_model_bigmap->setHeaderData(4,Qt::Horizontal,tr("经度"));
    table_model_bigmap->setHeaderData(5,Qt::Horizontal,tr("纬度"));
    //二、设置表格属性：
    ui->bigmap_table->setModel(table_model_bigmap);
    //设置表隔行变色
    ui->bigmap_table->setAlternatingRowColors(true);
    //设置每一次选中的是一整行也不是一个单独的item
    ui->bigmap_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    //表头信息显示居左
    ui->bigmap_table->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);
    ui->bigmap_table->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    //设置列宽可伸缩
    ui->bigmap_table->horizontalHeader()->setSectionResizeMode (0,QHeaderView::Stretch);
    ui->bigmap_table->horizontalHeader()->setSectionResizeMode (1,QHeaderView::Stretch);
    ui->bigmap_table->setColumnWidth(0,101);
    ui->bigmap_table->setColumnWidth(1,102);
    //设置选中时为整行选中
    ui->bigmap_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    //设置表格的单元为只读属性，即不能编辑
    ui->bigmap_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //如果你用在QTableView中使用右键菜单，需启用该属性
    ui->bigmap_table->setContextMenuPolicy(Qt::CustomContextMenu);
}

void MainWindow::importcsvfileGaode()
{
//    table_model_bigmap->clear();
    for(int i=table_model_bigmap->rowCount() - 1 ;i>-1; --i)
    {
        table_model_bigmap->removeRow(i);
    }
    QString fileName="D:\\AnimalSearch\\detectionresult.csv" ;
    QFile file(fileName);

    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qDebug()<<"Cannot open file for reading";
        return;
    }
    QStringList list;
    list.clear();
    QTextStream in(&file);
//    int i =0;
    while(!in.atEnd())
    {
        QString fileLine=in.readLine();
        list =fileLine.split(",",QString::SkipEmptyParts);
        int CurrentRow=table_model_bigmap->rowCount();
        {
            for (int item=0;item<6;item++)
            {
                table_model_bigmap->setItem(CurrentRow,item,new QStandardItem(list.at(item)));
                table_model_bigmap->item(CurrentRow,item)->setForeground(QBrush(QColor(0,0,0)));
            }
        }
    }
    file.close();
    qDebug()<<"主界面大地图导入detection.csv文件";

}

void MainWindow::bigmap_table_row_clicked(const QModelIndex &index)
{
    QAbstractItemModel *modessl =ui->bigmap_table->model();
    int curRow=index.row();
    QModelIndex indextem01=modessl->index(curRow,1);//遍历第一行的所有列
    QString datatemp01=modessl->data(indextem01).toString();
    datatemp01.replace('/',QChar('\\'));
    qDebug()<<"新地图信息datatemp01"<<datatemp01;
    QPixmap image;
    image.load(datatemp01);
     if(!(image.isNull()))
     {
         MapPath = datatemp01;
         qDebug()<<"新bigmao这是图片文件"<<datatemp01;
     }
     else{
         qDebug()<<"新bigmap这bu是图片文件"<<datatemp01;
     }

     //经度
     QModelIndex indextemp02 = modessl->index(curRow,4);//遍历第一行的所有列
     //这个是一个单元格的值。tostring()----ok
     QString datatemp02 = modessl->data(indextemp02).toString();
     qDebug()<<"新bigmap datatemp02:"<<datatemp02;

     //维度
     QModelIndex indextemp03 = modessl->index(curRow,5);//遍历第一行的所有列
     //这个是一个单元格的值。tostring()----ok
     QString datatemp03 = modessl->data(indextemp03).toString();
     qDebug()<<"datatemp03:"<<datatemp03;

     datatemp01="file://"+datatemp01;
     qDebug()<<"新bigmap测试输出01"<<datatemp01;
//      QString test01;

     qDebug()<<"测试输出01"<<datatemp01;
     QString test01 = "'"+datatemp01;
     QString test02 =test01+"'";
     qDebug()<<"测试输出02"<<test02;
    test02.replace('\\',('/'));
//      m_pWebView->page()->runJavaScript(QString("showtest(%1)").arg(test02));
//      m_pWebView->page()->runJavaScript("getPicPath_Gaode(%1,)",datatemp01); //调用参数
    big_WebView->page()->runJavaScript(QString("getPicPath_Gaode(%1,%2,%3)").arg(datatemp02).arg(datatemp03).arg(test02));
}

void MainWindow::bigmap_receiveJingWeiData(QString Jingdu,QString Weidu,QString path)
{
    qDebug()<<"新bigmap接收到信号数据"<<"+"<<Jingdu<<"+"<<Weidu<<"+"<<path;
    Jing=Jingdu;
    Wei=Weidu;

    Path="file://"+path;
    Path="'"+Path;
    Path=Path+"'";
    qDebug()<<"新bigmap输出"<<Path<<"..."<<Jing<<"..."<<Wei;
    qDebug()<<"bigmap_webview执行js脚本";
    big_WebView->page()->runJavaScript(QString("getPicPath_Gaode(%1,%2,%3)").arg(Jing).arg(Wei).arg(Path));


}

void MainWindow::onorigPositionChanged(qint64 position)
{//原始视频文件的播放进度改变，则改变进度条进度，是由一个信号激发的
    if(ui->origslider->isSliderDown())
        return;
    ui->origslider->setSliderPosition(position);

}

void MainWindow::onorigDurationChanged(qint64 duration)
{//播放器时长改变，就设置进度条的进度改变
    origplayer->setMuted(true);
    ui->origslider->setMaximum(duration);
}


void MainWindow::on_origslider_valueChanged(int value)//拖动进度条改变
{
    origplayer->setPosition(value);
}

void MainWindow::onresultPositionChanged(qint64 position)
{
    if(ui->resultslider->isSliderDown())
        return;
    ui->resultslider->setSliderPosition(position);
}

void MainWindow::onresultDurationChanged(qint64 duration)
{
    resultplayer->setMuted(true);
    ui->resultslider->setMaximum(duration);
}

void MainWindow::on_resultslider_valueChanged(int value)
{
    resultplayer->setPosition(value);
}

void MainWindow::on_setnewVideoSearchbtn_clicked()
{
    QMessageBox *videomessageBox=new QMessageBox(this);
    videomessageBox->setWindowTitle("请确认");
    videomessageBox->setText("请确认是否新建检索");
    videomessageBox->addButton("取消", QMessageBox::RejectRole);
    videomessageBox->addButton("确定", QMessageBox::AcceptRole);
    if(videomessageBox->exec()==QDialog::Accepted){
        ui->setnewVideoSearchbtn->setEnabled(false);//新建检索后，不能再次新建检索，防止删除未实例化的指针
        //处理上一次检测的指针变量，防止出现野指针及指针重定义的现象
        videoprogressTimer->stop();
        qDebug()<<"删除videoprogressTimer前"<<videoprogressTimer;
        delete videoprogressTimer;
        videoWatcher->removePath("D:/AnimalSearch/Videoresult");
        delete videoWatcher;
        delete resultplayer;
        videoWatcher->disconnect(SIGNAL(directoryChanged(QString)));
        this->disconnect(SIGNAL(sendFilepath(QString)));
        int videofilecount=table_model_videoSearch->rowCount();
        for(int row=0;row<videofilecount;row++){
            table_model_videoSearch->removeRow(0);
        }
        ui->progressBar->setValue(0);
        ui->startVideoSearch->setEnabled(true);

    }


}

void MainWindow::on_setnewPicSearch_clicked()
{
    QMessageBox *picmessageBox=new QMessageBox(this);
    picmessageBox->setWindowTitle("请确认");
    picmessageBox->setText("请确认是否新建检索");
    picmessageBox->addButton("取消",QMessageBox::RejectRole);
    picmessageBox->addButton("确定",QMessageBox::AcceptRole);
    if(picmessageBox->exec()==QDialog::Accepted){
        ui->setnewPicSearch->setEnabled(false);
        ui->StartSearchBtn->setEnabled(true);
        myWatcher->disconnect(SIGNAL(directoryChanged(QString)));
        delete myWatcher;
        this->disconnect(SIGNAL(sendFilepath(QString)));

        int picfilecount=table_model->rowCount();
        for(int row=0;row<picfilecount;row++){
            table_model->removeRow(0);
        }
        qDebug()<<"file name"<<filename;
        qDebug()<<"pic list"<<piclist;
        filename.clear();
        piclist.clear();
        qDebug()<<"file name clear"<<filename;
        qDebug()<<"pic list clear"<<piclist;
        int searchCount=table_model_SearchFile->rowCount();
        for(int i=0;i<searchCount;i++){
            table_model_SearchFile->removeRow(0);
        }
        int resCount=table_model_SearchResult->rowCount();
        for(int i=0;i<resCount;i++){
            table_model_SearchResult->removeRow(0);
        }//清空上次检测文件的文件列表
    }



}
