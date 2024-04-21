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

	/// @brief Constructor.
	/// @param type Type of glassware.
	Glassware(Glassware::Type type);

	/// @brief Destructor.
	~Glassware();

	const QPixmap& getDisplayPixmap() const;

	/// @brief Returns whether this glassware transparent or not.
	/// @return bool
	bool isTransparent() const;

	/// @brief Returns the coordinates of where the liquid should pour out.
	/// @return QPointF&
	const QPointF& getPhysicsPouringSource() const;

	/// @brief Returns a list of four vertices defining the collision layout of this glassware.
	/// Vertices ordered from top left to top right.
	/// @return QList<QPointF>&
	const QList<QPointF>& getPhysicsCollisionVertices() const;

private:
	QPixmap *displayPixmap;
	bool transparent;

	QPointF physicsPouringSource;
	QList<QPointF> physicsCollisionVertices;

};

#endif // GLASSWARE_H
