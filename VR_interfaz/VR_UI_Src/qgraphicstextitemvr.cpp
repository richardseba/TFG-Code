#include "qgraphicstextitemvr.h"

QGraphicsTextItemVR::QGraphicsTextItemVR()
{
    m_TextFont = QFont("Helvetica [Cronyx]",25,12,false );
    m_text = "";

    m_coordinates = QPoint(0,0);
    m_offset = QPoint(0,0);
}

QGraphicsTextItemVR::QGraphicsTextItemVR(QString text,
                                         QPoint coordinates,
                                         QPoint offset,
                                         QFont font)
{
    m_text = text;
    m_coordinates = coordinates;
    m_offset = offset;
    m_TextFont = font;
}

QGraphicsTextItemVR::QGraphicsTextItemVR(QString text, int x, int y,
                                         int offsetX, int offsetY,
                                         QFont font)
{
    m_text = text;
    m_coordinates = QPoint(x,y);
    m_offset = QPoint(offsetX,offsetY);
    m_TextFont = font;
}

QRectF QGraphicsTextItemVR::boundingRect() const
{
    qreal penWidth = 1;
    return QRectF(-10 - penWidth / 2, -10 - penWidth / 2,
                  20 + penWidth, 20 + penWidth); //to be done
}

void QGraphicsTextItemVR::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
           QWidget *widget)
{
    painter->setFont(m_TextFont);
    painter->drawText(m_coordinates,m_text);
    painter->drawText(m_coordinates+m_offset,m_text);
}


void QGraphicsTextItemVR::setText(QString text)
{
    m_text = text;
}

QString QGraphicsTextItemVR::getText()
{
    return m_text;
}

void QGraphicsTextItemVR::setFont(QFont font)
{
    m_TextFont = font;
}

QFont* QGraphicsTextItemVR::getFont()
{
    return &m_TextFont;
}

void QGraphicsTextItemVR::setCoordinates(QPoint coords)
{
    m_coordinates = coords;
}

void QGraphicsTextItemVR::moveBy(qreal dx, qreal dy)
{
    m_coordinates = QPoint(m_coordinates.x()+dx,m_coordinates.y()+dy);
}

QPoint QGraphicsTextItemVR::getCoordinates()
{
    return m_coordinates;
}

void QGraphicsTextItemVR::setOffset(int xOffset,int yOffset)
{
    m_offset = QPoint(xOffset,yOffset);
}

void QGraphicsTextItemVR::moveByOffset(qreal dx, qreal dy)
{
    m_offset.setX(m_offset.x()+dx);
    m_offset.setY(m_offset.y()+dy);
}

QPoint QGraphicsTextItemVR::getOffset()
{
    return m_offset;
}














