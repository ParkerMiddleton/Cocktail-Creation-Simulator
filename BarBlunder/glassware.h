#ifndef GLASSWARE_H
#define GLASSWARE_H

#include <QPixmap>

///
/// \brief The Glassware class creates different glassware polygons to be used
/// with Box2D. These act as containers for liquids.
///
/// Create a glassware object by passing in its corresponding QPolygonF from
/// barmodel datastructure
///
class Glassware
{
public:
	enum class Types { Rocks, Collins, Copper };

    /// \brief Glassware Constructor
    /// \param polygon Polygon to take the shape of the glass
    /// \param parent graphics scene that the object is housed in.
	Glassware();

protected:


private:



};

#endif // GLASSWARE_H
