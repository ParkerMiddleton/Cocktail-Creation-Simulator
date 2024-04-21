#ifndef GLASSWARE_H
#define GLASSWARE_H

#include <QPixmap>
#include <QPointF>

/// @brief Holds physics and rendering properties of various glasses.
class Glassware
{
public:
	enum class Type { Rocks, Collins, Copper, Martini };

	/**
	 * @brief Glassware -  constructor
	 * @param type - the type of glassware
	 */
	Glassware(Glassware::Type type);
	/// @brief the destructor for a glassware object
	~Glassware();
	/**
	 * @brief a method to ge the display Qpixmap
	 * @return the QPixmap of the display
	 */
	const QPixmap& getDisplayPixmap() const;
	/**
	 * @brief isTransparent - is glassware is transparen?
	 * @return if glassware is transparent or not
	 */
	bool isTransparent() const;
	/**
	 * @brief getPhysicsPouringSource - gets the source piint of the pouring
	 * @return the QPoint ref where the poiring is from
	 */
	const QPointF& getPhysicsPouringSource() const;
	/**
	 * @brief getPhysicsBottomRect - getting the bottom rect of the physics
	 * @return the ref to the QRectF at bottom
	 */
	const QRectF& getPhysicsBottomRect() const;
	/**
	 * @brief getPhysicsLeftRect - gets left rect of the physics
	 * @return the ref to the QRectF at left
	 */
	const QRectF& getPhysicsLeftRect() const;
	/**
	 *@brief getPhysicsLeftRect - gets right rect of the physics
	 * @return the ref to the QRectF at right
	 */
	const QRectF& getPhysicsRightRect() const;

private:
	/// @brief the display pixmap
	QPixmap *displayPixmap;
	/// @brief is glassware transparent?
	bool transparent;
	/// @brief the QPoinF where the pouring is from
	QPointF physicsPouringSource;
	/// @brief the ref to the QRectF at bottom
	QRectF physicsBottomRect;
	/// @brief the ref to the QRectF at left
	QRectF physicsLeftRect;
	/// @brief the ref to the QRectF at right
	QRectF physicsRightRect;

};

#endif // GLASSWARE_H
