#pragma execution_character_set("utf-8")
#include<tabwidget.h>
#include<QWidget>
tabwidget::tabwidget(QObject *parent)
{

    QTabBar *bar = new QTabBar;
    bar->setStyle(new CustomTabStyle);
    setTabBar(bar);


//    QWidget *w = new QWidget;
//    bar->addTab(QIcon(""),"123");
//    w = new QWidget;
//    bar->addTab(QIcon(""),"9");




}


tabwidget::~tabwidget()
{


}
