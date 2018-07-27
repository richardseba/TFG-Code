#ifndef QTFILTER_H
#define QTFILTER_H

#include <QDebug>
#include <QEvent>
#include <QTime>

/* Class QtFilter
 * -------------------------------
 * Class used to debug the QT's event system
 *
 * it inherits from QObject
 * Uses Q_OBJECT macro
*/
class QtFilter : public QObject
{
    Q_OBJECT
protected:
    bool eventFilter(QObject *obj, QEvent *event)
    {
        bool ret = QObject::eventFilter(obj, event);
        qDebug() << event->type();
        return ret;
    }
};

#endif // QTFILTER_H
