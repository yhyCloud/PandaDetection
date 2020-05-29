#ifndef DETECTIONTHREAD_CMD_H
#define DETECTIONTHREAD_CMD_H

#include <QObject>
#include <QThread>

class DetectionThread_cmd : public QThread
{
    Q_OBJECT
public:
    explicit DetectionThread_cmd(QObject *parent = nullptr);
protected :
   void run();

signals:

public slots:
};

#endif // DETECTIONTHREAD_CMD_H
