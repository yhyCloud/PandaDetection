#ifndef DETECTIONTHREAD_H
#define DETECTIONTHREAD_H

#include <QObject>
#include <QThread>

class DetectionThread : public QThread
{
    Q_OBJECT
public:
    explicit DetectionThread(QObject *parent = nullptr);

protected:
    void run();//

signals:

public slots:
};

#endif // DETECTIONTHREAD_H
