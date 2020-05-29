#pragma execution_character_set("utf-8")
#include "VideodetectThread.h"
#include <Python.h>
#include <QDebug>
VideodetectThread::VideodetectThread(QObject *parent) : QThread(parent)
{

}

void VideodetectThread::run()
{
    qDebug()<<"开始检测";
    Py_Initialize();
    if(Py_IsInitialized())
    {
        qDebug()<<"初始化成功";
        PyRun_SimpleString("import sys");

        PyRun_SimpleString("sys.path.append('D:/AnimalSearch/Pandadetection')");//导入py脚本路径
//        PyRun_SimpleString("sys.path.append('D:/AnimalSearch/Pandadetection')");

        PyObject *pModule=false;
        pModule=PyImport_ImportModule("videodetect");//获取python脚本
//        pModule = PyImport_ImportModule("detection_v2");
        if(pModule)
        {
            qDebug()<<"获取python脚本成功";
            PyObject *pFun=false;
            pFun=PyObject_GetAttrString(pModule,"main");//获取脚本中的函数
            if(pFun)
            {
                qDebug()<<"获取函数成功";
//                PyObject* pArg=Py_BuildValue("(ss)","D:/Pycharmcode/Pandadetection_v2/pandavideo.mp4","E:/videoresult.avi");
                PyObject* pArg=Py_BuildValue("(s)","D:/AnimalSearch/videoPath.txt");
                PyEval_CallObject(pFun,pArg);//执行python函数
            }
            else{
                qDebug()<<"获取函数失败";
            }
        }
        else {
            qDebug()<<"获取脚本失败";
        }
    }
    else {
        qDebug()<<"初始化失败";
    }
    Py_Finalize();
}
