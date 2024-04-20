#include "glassware.h"

Glassware::Glassware(Glassware::Type type)
{
	//
	// !!!!! For now all of them are rocks. !!!!!
	//

	if (type == Glassware::Type::Rocks)
	{
		displayPixmap = new QPixmap(":/images/glasses_liquid/rocks_liquid.png");
		transparent = true;

		physicsPouringSource.setX(120.0f);
		physicsPouringSource.setY(165.0f);

        physicsBottomRect.setRect(85.0f,  236.0f, 89.0f, 15.0f);
        physicsLeftRect.setRect(83.0f, 154.0f, 9.0f, 82.0f);
        physicsRightRect.setRect(167.0f, 154.0f, 9.0f, 82.0f);
	}
	else if (type == Glassware::Type::Collins)
	{
		displayPixmap = new QPixmap(":/images/glasses_liquid/collins_liquid.png");
		transparent = true;

		physicsPouringSource.setX(120.0f);
		physicsPouringSource.setY(165.0f);

		physicsBottomRect.setRect(89.0f,  236.0f, 64.0f, 15.0f);
		physicsLeftRect.setRect(80.0f, 168.0f, 9.0f, 68.0f);
		physicsRightRect.setRect(153.0f, 168.0f, 9.0f, 68.0f);
	}
	else if (type == Glassware::Type::Copper)
	{
		displayPixmap = new QPixmap(":/images/glasses_liquid/copper_liquid.png");
		transparent = false;

		physicsPouringSource.setX(120.0f);
		physicsPouringSource.setY(165.0f);

		physicsBottomRect.setRect(89.0f,  236.0f, 64.0f, 15.0f);
		physicsLeftRect.setRect(80.0f, 168.0f, 9.0f, 68.0f);
		physicsRightRect.setRect(153.0f, 168.0f, 9.0f, 68.0f);
	}
	else if (type == Glassware::Type::Martini)
	{
		displayPixmap = new QPixmap(":/images/glasses_liquid/martini_liquid.png");
		transparent = true;

		physicsPouringSource.setX(120.0f);
		physicsPouringSource.setY(165.0f);

		physicsBottomRect.setRect(89.0f,  236.0f, 64.0f, 15.0f);
		physicsLeftRect.setRect(80.0f, 168.0f, 9.0f, 68.0f);
		physicsRightRect.setRect(153.0f, 168.0f, 9.0f, 68.0f);
	}
}

Glassware::~Glassware()
{
	delete displayPixmap;
}

const QPixmap& Glassware::getDisplayPixmap() const
{
	return *displayPixmap;
}

bool Glassware::isTransparent() const
{
	return transparent;
}

const QPointF& Glassware::getPhysicsPouringSource() const
{
	return physicsPouringSource;
}

const QRectF& Glassware::getPhysicsBottomRect() const
{
	return physicsBottomRect;
}

const QRectF& Glassware::getPhysicsLeftRect() const
{
	return physicsLeftRect;
}

const QRectF& Glassware::getPhysicsRightRect() const
{
	return physicsRightRect;
}
