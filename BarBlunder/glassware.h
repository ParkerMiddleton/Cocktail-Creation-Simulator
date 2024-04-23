#ifndef GLASSWARE_H
#define GLASSWARE_H

#include <QPixmap>
#include <QPointF>
#include <QList>

/// @brief Holds physics and rendering properties of various glasses.
class Glassware
{
public:
	enum class Type { Rocks, Collins, Copper, Martini };
	enum class Garnish { None, Olive, Lime, Orange };

	/// @brief Constructor.
	/// @param type Type of glassware.
	Glassware(Glassware::Type type);

	/// @brief Destructor.
	~Glassware();

	/// @brief Returns pixmap of this glassware.
	/// @return const QPixmap&
	const QPixmap& getDisplayPixmap() const;

	/// @brief Returns whether this glassware is transparent or not.
	/// @return bool
	bool isTransparent() const;

	/// @brief Places the given garnish on the glassware.
	/// @param garnish Type of garnish to place.
	void placeGarnish(Glassware::Garnish garnish);

	/// @brief Returns the coordinates of where the liquid should pour out.
	/// @return QPointF&
	const QPointF& getPhysicsPouringSource() const;

	/// @brief Returns a list of four vertices defining the collision layout of this glassware.
	/// Vertices ordered from top left to top right.
	/// @return QList<QPointF>&
	const QList<QPointF>& getPhysicsCollisionVertices() const;

private:
	QPixmap *displayPixmap;

	QPixmap *glasswarePixmap;
	QPixmap *olivePixmap;
	QPixmap *limePixmap;
	QPixmap *orangePixmap;

	bool transparent;

	QPointF physicsPouringSource;
	QList<QPointF> physicsCollisionVertices;

};

#endif // GLASSWARE_H
