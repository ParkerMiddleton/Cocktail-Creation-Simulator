#include "glassware.h"

Glassware::Glassware(Glassware::Type type)
{
	if (type == Glassware::Type::Rocks)
	{
		displayPixmap = new QPixmap(":/images/glasses_liquid/rocks_liquid.png");
		transparent = true;

		physicsPouringSource.setX(130.0f);
		physicsPouringSource.setY(145.0f);

		// Always Four Vertices.
		// Starting from top left.
		physicsCollisionVertices.push_back({88.0f, 156.0f});
		physicsCollisionVertices.push_back({93.0f, 235.0f});
		physicsCollisionVertices.push_back({167.0f, 235.0f});
		physicsCollisionVertices.push_back({172.0f, 156.0f});
	}
	else if (type == Glassware::Type::Collins)
	{
		displayPixmap = new QPixmap(":/images/glasses_liquid/collins_liquid.png");
		transparent = true;

		physicsPouringSource.setX(130.0f);
		physicsPouringSource.setY(57.0f);

		// Always Four Vertices.
		// Starting from top left.
		physicsCollisionVertices.push_back({86.0f, 75.0f});
		physicsCollisionVertices.push_back({104.0f, 249.0f});
		physicsCollisionVertices.push_back({154.0f, 249.0f});
		physicsCollisionVertices.push_back({171.0f, 75.0f});
	}
	else if (type == Glassware::Type::Copper)
	{
		displayPixmap = new QPixmap(":/images/glasses_liquid/copper_liquid.png");
		transparent = false;

		physicsPouringSource.setX(130.0f);
		physicsPouringSource.setY(165.0f);

		// Always Four Vertices.
		// Starting from top left.
		physicsCollisionVertices.push_back({88.0f, 156.0f});
		physicsCollisionVertices.push_back({93.0f, 235.0f});
		physicsCollisionVertices.push_back({163.0f, 235.0f});
		physicsCollisionVertices.push_back({170.0f, 156.0f});
	}
	else if (type == Glassware::Type::Martini)
	{
		displayPixmap = new QPixmap(":/images/glasses_liquid/martini_liquid.png");
		transparent = true;

		physicsPouringSource.setX(130.0f);
		physicsPouringSource.setY(71.0f);

		// Always Four Vertices.
		// Starting from top left.
		physicsCollisionVertices.push_back({69.0f, 88.0f});
		physicsCollisionVertices.push_back({122.0f, 158.0f});
		physicsCollisionVertices.push_back({140.0f, 158.0f});
		physicsCollisionVertices.push_back({193.0f, 88.0f});
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

const QList<QPointF>& Glassware::getPhysicsCollisionVertices() const
{
	return physicsCollisionVertices;
}
