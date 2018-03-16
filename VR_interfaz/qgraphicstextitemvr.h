#ifndef QGRAPHICSTEXTITEMVR_H
#define QGRAPHICSTEXTITEMVR_H
#include <QGraphicsTextItem>
#include <QGraphicsItem>
#include <QFont>
#include <QPainter>
#include <QDebug>

/* Class QGraphicsTextItemVR
 * -------------------------------
 * This class is used to display a text in an VR setup, therefore it displays
 * to images with a given offset
 *
 * it inherits from QGraphicsItem
*/
class QGraphicsTextItemVR : public QGraphicsItem
{
public:
    QGraphicsTextItemVR();
    QGraphicsTextItemVR(QString text, QPoint coordinates = QPoint(0,0),
                        QPoint offset = QPoint(0,0), QFont font=QFont());
    QGraphicsTextItemVR(QString text, int x , int y ,
                        int offsetX = 0, int offsetY = 0,
                        QFont font=QFont());

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    void setText(QString text);
    QString getText();

    void setFont(QFont font);
    QFont* getFont();

    void setCoordinates(QPoint coords);
    void moveBy(qreal dx, qreal dy);
    QPoint getCoordinates();

    void setOffset(int xOffset,int yOffset);
    void moveByOffset(qreal dx,qreal dy);
    QPoint getOffset();
private:
    QPoint m_coordinates;
    QPoint m_offset;

    QString m_text;
    QFont m_TextFont;
};

#endif // QGRAPHICSTEXTITEMVR_H
