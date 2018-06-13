#include "vrui.h"


VrUI::VrUI()
{
    QGraphicsItemGroup::QGraphicsItemGroup();
    m_uiFont = new QFont("Helvetica [Cronyx]",25,12,false );
    m_fpsText1 = new QGraphicsTextItem("haodsfdsdfsdfsdfs!");
    m_fpsText1->setFont(*m_uiFont);
    this->addToGroup(m_fpsText1);

    QPolygonF* polyNose = new QPolygonF();
    polyNose->append(QPointF(0,0));
    polyNose->append(QPointF(0,-300));
    polyNose->append(QPointF(-150,0));
    m_nose = new QGraphicsPolygonItem(*polyNose);
    QBrush noseBrush = QBrush();
    noseBrush.setStyle(Qt::BrushStyle::SolidPattern);
    noseBrush.setColor(Qt::GlobalColor::blue);
    m_nose->setBrush(noseBrush);
    m_nose->setPen(QPen(noseBrush,0));
    this->addToGroup(m_nose);
}
VrUI::~VrUI()
{
}
