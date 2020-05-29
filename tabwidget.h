#ifndef TABWIDGET_H
#define TABWIDGET_H


#include <QTabWidget>
#include <QTabBar>
#include <QProxyStyle>
#include <QPainter>
#include <QStyleOption>


class CustomTabStyle : public QProxyStyle
{
    public:
    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
    const QSize &size, const QWidget *widget) const
    {
        QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
        if (type == QStyle::CT_TabBarTab)
        {
        s.transpose();
           s.rwidth()=120;
           s.rheight()=40;
        }
        return s;
    }


    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        if (element == CE_TabBarTabLabel)
        {
            if (const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option))
            {
                QStyleOptionTab opt(*tab);
                opt.shape = QTabBar::RoundedNorth;
                //opt.text = tr("Hello");
                //QIcon icon(":/Resources/icon2.ico");
                //opt.icon = icon;
                opt.palette.setCurrentColorGroup(QPalette::Disabled);
                opt.state |= QStyle::State_Sunken;
                QProxyStyle::drawControl(element, &opt, painter, widget);
                return;
            }
        }


        QProxyStyle::drawControl(element, option, painter, widget);
    }
};


class tabwidget : public QTabWidget
{
Q_OBJECT


    public:
    tabwidget(QObject *parent = NULL);
    ~tabwidget();


    private:

};

#endif // TABWIDGET_H
