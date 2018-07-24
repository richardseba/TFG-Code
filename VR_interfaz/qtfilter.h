#ifndef QTFILTER_H
#define QTFILTER_H

#include <QDebug>
#include <QEvent>
#include <QTime>

class QtFilter : public QObject
{
    Q_OBJECT
protected:
    bool eventFilter(QObject *obj, QEvent *event)
    {
        bool ret = QObject::eventFilter(obj, event);
//        qDebug() << event->type();
        return ret;
    }
};

#endif // QTFILTER_H
