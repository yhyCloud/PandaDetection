#pragma execution_character_set("utf-8")
#include "DetectionThread.h"
#include<QDebug>
#include<Python.h>
DetectionThread::DetectionThread(QObject *parent) : QThread(parent)
{

}

void DetectionThread::run()
{
    qDebug()<<"开始检测";
    Py_Initialize();
    if(Py_IsInitialized())
    {
        qDebug()<<"初始化成功";
        PyRun_SimpleString("import sys");
        qDebug()<<"导入sys";
        PyRun_SimpleString("sys.path.append('D:/AnimalSearch/Pandadetection')");
        qDebug()<<"导入py路径";
        PyObject *pModule=NULL;

        qDebug()<<"py脚本指针"<<pModule;
        pModule = PyImport_ImportModule("detection_v2");//获取python脚本
        qDebug()<<"获取python脚本"<<pModule;
        if(pModule)
        {
         PyObject *pFun=false;
         pFun=PyObject_GetAttrString(pModule,"main");//获取脚本中的函数
         if(pFun)
         {
             PyObject* pArg=Py_BuildValue("(s)","D:/AnimalSearch/filePath.txt");//建立给python传入的执行参数
             PyEval_CallObject(pFun,pArg);//执行
             qDebug()<<"运行结束后py脚本指针"<<pModule;
//             delete(pArg);
//             delete(pFun);
//             delete(pModule);
             Py_DECREF(pArg);
             Py_DECREF(pFun);
             Py_DECREF(pModule);
             pArg=NULL;
             pFun=NULL;
             pModule=NULL;
             qDebug()<<pArg;
             qDebug()<<pFun;
             qDebug()<<pModule;


         }
         else {
             qDebug()<<"获取函数失败";
         }
        }
        else {
            qDebug("获取脚本失败");
        }
    }
    else {
        qDebug()<<"初始化失败";
    }
    Py_Finalize();
    qDebug()<<"Py_Finalize()";



}
