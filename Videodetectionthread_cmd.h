#ifndef VIDEODETECTIONTHREAD_CMD_H
#define VIDEODETECTIONTHREAD_CMD_H

#include <QObject>
#include <QThread>

class Videodetectionthread_cmd : public QThread
{
    Q_OBJECT
public:
    explicit Videodetectionthread_cmd(QObject *parent = nullptr);

signals:
protected:
    void run();
public slots:
};

#endif // VIDEODETECTIONTHREAD_CMD_H
