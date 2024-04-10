#include "gamepage.h"
#include "ui_gamepage.h"

GamePage::GamePage(QWidget *parent)
	: QWidget{parent}
	, ui{new Ui::GamePage}
{
	ui->setupUi(this);

	////////////// ICONS FOR BUTTONS /////////////////////

	// background image
	QPixmap bkgImage(":/images/bar.png");
	int backgroundHeight = 400;
	int backgroundWidth = 1156;
	bkgImage = bkgImage.scaled(backgroundWidth, backgroundHeight, Qt::KeepAspectRatioByExpanding);
	ui->backgroundImage->setPixmap(bkgImage);


	//Vodka
	QIcon vodkaIcon(":/images/titos.png");
	ui->VodkaButton->setIcon(vodkaIcon);
	ui->VodkaButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	QSize vodkaButtonSize(ui->VodkaButton->size());
	ui->VodkaButton->setIconSize(vodkaButtonSize * 2.85);

	//Tequila
	QIcon tequilaIcon(":/images/tequila.png");
	ui->TequilaButton->setIcon(tequilaIcon);
	ui->TequilaButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	QSize tequilaButtonSize(ui->TequilaButton->size());
	ui->TequilaButton->setIconSize(tequilaButtonSize * 2.8);

	//Gin
	QIcon ginIcon(":/images/gin.png");
	ui->GinButton->setIcon(ginIcon);
	ui->GinButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	QSize ginButtonSize(ui->GinButton->size());
	ui->GinButton->setIconSize(ginButtonSize * .85);

	//Burbon
	QIcon burbonIcon(":/images/burbon.png");
	ui->BurbonButton->setIcon(burbonIcon);
	ui->BurbonButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	QSize burbonButtonSize(ui->BurbonButton->size());
	ui->BurbonButton->setIconSize(burbonButtonSize * 2.3);

	//Rum
	QIcon rumIcon(":/images/rum.png");
	ui->RumButton->setIcon(rumIcon);
	ui->RumButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	QSize rumButtonSize(ui->RumButton->size());
	ui->RumButton->setIconSize(rumButtonSize * 2.5);

	//Lime Wedge Container
	QIcon limeIcon(":/images/lime-wedges-in-glass.png");
	ui->LimeButton->setIcon(limeIcon);
	ui->LimeButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	QSize limeButtonSize(ui->LimeButton->size());
	ui->LimeButton->setIconSize(limeButtonSize*1.3);

	// Orange Peele container
	QIcon orangeIcon(":/images/orangePeeles.png");
	ui->OrangeButton->setIcon(orangeIcon);
	ui->OrangeButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	QSize orangeButtonSize(ui->OrangeButton->size());
	ui->OrangeButton->setIconSize(orangeButtonSize*1.7);

	// Olive container
	QIcon oliveIcon(":/images/olives.png");
	ui->OliveButton->setIcon(oliveIcon);
	ui->OliveButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	QSize oliveButtonSize(ui->OliveButton->size());
	ui->OliveButton->setIconSize(oliveButtonSize*1.7);

	//bitters bottle
	QIcon bittersIcon(":/images/bitters.png");
	ui->BittersButton->setIcon(bittersIcon);
	ui->BittersButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	QSize BittersButtonSize(ui->BittersButton->size());
	ui->BittersButton->setIconSize(BittersButtonSize*1.7);

	//shakertin
	QIcon shakertinIcon(":/images/shakertin.png");
	ui->ShakertinButton->setIcon(shakertinIcon);
	ui->ShakertinButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	QSize shakertinButtonSize(ui->ShakertinButton->size());
	ui->ShakertinButton->setIconSize(shakertinButtonSize*.95);

	// ice cubes
	QIcon iceIcon(":/images/ice.png");
	ui->IceButton->setIcon(iceIcon);
	ui->IceButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	QSize iceButtonSize(ui->IceButton->size());
	ui->IceButton->setIconSize(iceButtonSize * 1.3);

	// Connections
	connect(ui->BackButton, &QPushButton::clicked,
			this, &GamePage::onBackButtonClicked);
}

GamePage::~GamePage()
{
	delete ui;
}

void GamePage::onBackButtonClicked()
{
	emit gameExitRequested();
}
