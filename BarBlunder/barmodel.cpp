#include "barmodel.h"

BarModel::BarModel(QObject *parent)
	: QObject{parent}
{


}

void BarModel::initialize()
{
	emit barOpened();
}
