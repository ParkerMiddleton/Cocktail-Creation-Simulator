#include "Glassware.h"


Glassware::Glassware(const QPolygonF& polygon, QGraphicsItem *parent)
: QGraphicsPolygonItem(polygon, parent){

    m_brush = QBrush(Qt::white);
     m_pen = QPen(Qt::black);
     m_pen.setWidth(2);
}

QRectF Glassware::boundingRect() const
{
    // Return the bounding rectangle of the polygon
    return QGraphicsPolygonItem::boundingRect();
}

void Glassware::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // Set the brush and pen for painting
    painter->setBrush(m_brush);
    painter->setPen(m_pen);

    // Paint the polygon
    painter->drawPolygon(polygon());
}


