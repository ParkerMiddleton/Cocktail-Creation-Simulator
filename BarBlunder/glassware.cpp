#include "glassware.h"

Glassware::Glassware(Glassware::Type type)
{
	if (type == Glassware::Type::Rocks)
	{
		glasswarePixmap = new QPixmap(":/images/maindrink/rocks.png");
		olivePixmap = new QPixmap(":/images/maindrink/rocks_olive.png");
		limePixmap = new QPixmap(":/images/maindrink/rocks_lime.png");
		orangePixmap = new QPixmap(":/images/maindrink/rocks_orange.png");

		displayPixmap = glasswarePixmap;
		transparent = true;

		physicsPouringSource.setX(130.0f);
		physicsPouringSource.setY(110.0f);

		// Always Four Vertices.
		// Starting from top left.
		physicsCollisionVertices.push_back({88.0f, 156.0f});
		physicsCollisionVertices.push_back({93.0f, 235.0f});
		physicsCollisionVertices.push_back({167.0f, 235.0f});
		physicsCollisionVertices.push_back({174.0f, 156.0f});
	}
	else if (type == Glassware::Type::Collins)
	{
		glasswarePixmap = new QPixmap(":/images/maindrink/collins.png");
		olivePixmap = new QPixmap(":/images/maindrink/collins_olive.png");
		limePixmap = new QPixmap(":/images/maindrink/collins_lime.png");
		orangePixmap = new QPixmap(":/images/maindrink/collins_orange.png");

		displayPixmap = glasswarePixmap;
		transparent = true;

		physicsPouringSource.setX(130.0f);
		physicsPouringSource.setY(0.0f);

		// Always Four Vertices.
		// Starting from top left.
		physicsCollisionVertices.push_back({86.0f, 75.0f});
		physicsCollisionVertices.push_back({97.0f, 249.0f});
		physicsCollisionVertices.push_back({163.0f, 249.0f});
		physicsCollisionVertices.push_back({173.0f, 75.0f});
	}
	else if (type == Glassware::Type::Copper)
	{
		glasswarePixmap = new QPixmap(":/images/maindrink/copper.png");
		olivePixmap = new QPixmap(":/images/maindrink/copper_olive.png");
		limePixmap = new QPixmap(":/images/maindrink/copper_lime.png");
		orangePixmap = new QPixmap(":/images/maindrink/copper_orange.png");

		displayPixmap = glasswarePixmap;
		transparent = false;

		physicsPouringSource.setX(130.0f);
		physicsPouringSource.setY(0.0f);

		// Always Four Vertices.
		// Starting from top left.
		physicsCollisionVertices.push_back({87.0f, 122.0f});
		physicsCollisionVertices.push_back({91, 261.0f});
		physicsCollisionVertices.push_back({169.0f, 261.0f});
		physicsCollisionVertices.push_back({170.0f, 122.0f});
	}
	else if (type == Glassware::Type::Martini)
	{
		glasswarePixmap = new QPixmap(":/images/maindrink/martini.png");
		olivePixmap = new QPixmap(":/images/maindrink/martini_olive.png");
		limePixmap = new QPixmap(":/images/maindrink/martini_lime.png");
		orangePixmap = new QPixmap(":/images/maindrink/martini_orange.png");

		displayPixmap = glasswarePixmap;
		transparent = true;

		physicsPouringSource.setX(166.0f);
		physicsPouringSource.setY(68.0f);

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
	delete glasswarePixmap;
	delete olivePixmap;
	delete limePixmap;
	delete orangePixmap;
}

const QPixmap& Glassware::getDisplayPixmap() const
{
	return *displayPixmap;
}


bool Glassware::isTransparent() const
{
	return transparent;
}

void Glassware::placeGarnish(Garnish garnish)
{
	switch (garnish)
	{
	case Glassware::Garnish::None:
		displayPixmap = glasswarePixmap;
		break;
	case Glassware::Garnish::Olive:
		displayPixmap = olivePixmap;
		break;
	case Glassware::Garnish::Lime:
		displayPixmap = limePixmap;
		break;
	case Glassware::Garnish::Orange:
		displayPixmap = orangePixmap;
		break;
	}
}

const QPointF& Glassware::getPhysicsPouringSource() const
{
	return physicsPouringSource;
}

const QList<QPointF>& Glassware::getPhysicsCollisionVertices() const
{
	return physicsCollisionVertices;
}
