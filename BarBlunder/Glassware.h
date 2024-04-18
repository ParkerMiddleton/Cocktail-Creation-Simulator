#ifndef GLASSWARE_H
#define GLASSWARE_H
#include <QGraphicsItem>
#include <QPainter>
#include <QWidget>
#include <QMouseEvent>


///
/// \brief The Glassware class creates different glassware polygons to be used
/// with Box2D. These act as containers for liquids.
///
/// Create a glassware object by passing in its corresponding QPolygonF from
/// barmodel datastructure
///
class Glassware : public QGraphicsPolygonItem
{
public:

    ///
    /// \brief Glassware Constructor
    /// \param polygon Polygon to take the shape of the glass
    /// \param parent graphics scene that the object is housed in.
    ///
    Glassware(const QPolygonF& polygon, QGraphicsItem *parent = nullptr);

protected:

    //Overrides required for implementing QGraphicsPolygonItem
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

 private:

    QBrush m_brush;
    QPen m_pen;



};

#endif // GLASSWARE_H
