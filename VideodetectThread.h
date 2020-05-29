#ifndef VIDEODETECTTHREAD_H
#define VIDEODETECTTHREAD_H

#include <QObject>
#include <QThread>
class VideodetectThread : public QThread
{
    Q_OBJECT
public:
    explicit VideodetectThread(QObject *parent = nullptr);

protected:
    void run();

signals:

public slots:
};

#endif // VIDEODETECTTHREAD_H
