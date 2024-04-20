#ifndef GLASSWARE_H
#define GLASSWARE_H

#include <QPixmap>
#include <QPointF>

/// @brief Holds physics and rendering properties of various glasses.
class Glassware
{
public:
	enum class Type { Rocks, Collins, Copper, Martini };

	/// \brief Constructor
	Glassware(Glassware::Type type);
	~Glassware();

	const QPixmap& getDisplayPixmap() const;
	bool isTransparent() const;

	const QPointF& getPhysicsPouringSource() const;
	const QRectF& getPhysicsBottomRect() const;
	const QRectF& getPhysicsLeftRect() const;
	const QRectF& getPhysicsRightRect() const;

private:
	QPixmap *displayPixmap;
	bool transparent;

	QPointF physicsPouringSource;
	QRectF physicsBottomRect;
	QRectF physicsLeftRect;
	QRectF physicsRightRect;

};

#endif // GLASSWARE_H
