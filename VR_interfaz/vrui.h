#ifndef VRUI_H
#define VRUI_H
#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QPointF>
#include <QVector>
#include <QPolygonF>
#include <QGraphicsPolygonItem>
#include "QGraphicsScene"
#include <QDebug>
#include <QGraphicsItemGroup>



/* Class MainWindow
 * -------------------------------
 * This class is mean to be the main controller of a future VR UI
 * currently is empty in functionality
 *
*/
class VrUI : public QGraphicsItemGroup
{
public:
    VrUI();
    ~VrUI();

private:
    QGraphicsPolygonItem* m_nose;
    QGraphicsTextItem* m_fpsText1;
    QGraphicsRectItem* m_rectangle;
    QFont* m_uiFont;

};

#endif // VRUI_H
