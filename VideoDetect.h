#ifndef VIDEODETECT_H
#define VIDEODETECT_H

#include <QObject>
#include <QThread>

class VideoDetect : public QObject
{
    Q_OBJECT
public:
    explicit VideoDetect(QObject *parent = nullptr);

signals:

public slots:
};

#endif // VIDEODETECT_H
