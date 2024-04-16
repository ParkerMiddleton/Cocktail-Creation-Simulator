#include "mainmenupage.h"
#include "ui_mainmenupage.h"

#include "menubutton.h"

MainMenuPage::MainMenuPage(QWidget *parent)
	: QWidget{nullptr}
	, ui{new Ui::MainMenuPage}
{
	ui->setupUi(this);

	pages = ui->pages;
	this->setupMenuPage();
	this->setupSettingsPage();
	this->setupAnimations();

	pages->addWidget(menuPage);
	pages->addWidget(settingsPage);

	pages->setCurrentWidget(menuPage);

	/*QPixmap backgroundImage(":/images/bar.png");
	QPixmap scaledImage = backgroundImage.scaled(800, 800, Qt::KeepAspectRatio);
	ui->menuBarImage->setPixmap(scaledImage);
	ui->menuBarImage->setAlignment(Qt::AlignCenter);*/
	ui->menuBarImage->setVisible(false);
}

MainMenuPage::~MainMenuPage()
{
	delete resumeButton;
	delete beginButton;
	delete settingsButton;
	delete quitButton;

	delete ui;
}

void MainMenuPage::show()
{
	// If edu is not started.
	if (!resumeButton->isVisible())
	{
		resumeButton->setVisible(true);
        beginButton->setText("RESTART GAME");
        // somehow inform gamemodel to call new game
	}

	triAnim->setDirection(QAbstractAnimation::Backward);
	triAnim->setEasingCurve(QEasingCurve::InCirc);

	if (triAnim->state() == QAbstractAnimation::Stopped)
	{
		triAnim->start();
	}

	pages->setVisible(true);

	if (fadeOutAnim->state() == QAbstractAnimation::Stopped)
	{
		fadeOutAnim->start();
	}
}

void MainMenuPage::hide()
{
	triAnim->setDirection(QAbstractAnimation::Forward);
	triAnim->setEasingCurve(QEasingCurve::InSine);

	if (triAnim->state() == QAbstractAnimation::Stopped)
	{
		triAnim->start();
	}

	if (fadeOutAnim->state() == QAbstractAnimation::Running)
	{
		fadeOutAnim->stop();
	}

	if (pages->currentWidget() == settingsPage)
	{
		pages->setCurrentWidget(menuPage);
	}

	pages->setVisible(false);
}

void MainMenuPage::onResumeButtonClicked()
{
	emit unpauseRequested();
}

void MainMenuPage::onBeginButtonClicked()
{
    emit beginAnewRequested();
}

void MainMenuPage::onSettingsButtonClicked()
{
	pages->setCurrentWidget(settingsPage);
}

void MainMenuPage::onQuitButtonClicked()
{
	emit quitRequested();
}

void MainMenuPage::onBackButtonClicked()
{
	pages->setCurrentWidget(menuPage);
}

void MainMenuPage::setupMenuPage()
{
	menuPage = new QWidget();
	menuPage->setStyleSheet("background-color: transparent");

	titleLabel = new QLabel(menuPage);
	titleLabel->setGeometry(120, 120, 311, 71);
	titleLabel->setText("Bar Blunder //PLACEHOLDER");
	titleLabel->setStyleSheet("background:rgb(255,255,255);"\
							  "border: 4px solid;"\
							  "border-color:rgb(150, 230, 230);");

	resumeButton = new MenuButton(136, 338, "RESUME", menuPage);
	resumeButton->setVisible(false);
	beginButton = new MenuButton(136, 412, "BEGIN", menuPage);
	settingsButton = new MenuButton(136, 470, "SETTINGS", menuPage);
	quitButton = new MenuButton(136, 528, "QUIT", menuPage);

	connect(resumeButton, &QPushButton::clicked,
			this, &MainMenuPage::onResumeButtonClicked);

	connect(beginButton, &QPushButton::clicked,
			this, &MainMenuPage::onBeginButtonClicked);

	connect(settingsButton, &QPushButton::clicked,
			this, &MainMenuPage::onSettingsButtonClicked);

	connect(quitButton, &QPushButton::clicked,
			this, &MainMenuPage::onQuitButtonClicked);
}

void MainMenuPage::setupSettingsPage()
{
	settingsPage = new QWidget();
	settingsPage->setStyleSheet("background-color: transparent;");

	volumeSpinBox = new QSpinBox(settingsPage);
	volumeSpinBox->setGeometry(136, 383, 236, 40);
	volumeSpinBox->setStyleSheet("QSpinBox {"\
								 "  border: 1px solid #ABABAB;"\
								 "  border-radius: 3px;"\
								 "}"\
								 ""\
								 "QSpinBox::up-button  {"\
								 "  subcontrol-origin: margin;"\
								 "  subcontrol-position: center left;"\
								 "  image: url(:/icons/leftArrow.png);"\
								 "  background-color: #ABABAB;"\
								 "  left: 1px;"\
								 "  height: 24px;"\
								 "  width: 24px;"\
								 "}"\
								 ""\
								 "QSpinBox::down-button  {"\
								 "  subcontrol-origin: margin;"\
								 "  subcontrol-position: center right;"\
								 "  image: url(:/icons/rightArrow.png);"\
								 "  background-color: #ABABAB;"\
								 "  right: 1px;"\
								 "  height: 24px;"\
								 "  width: 24px;"\
								 "}");

	windowSizeComboBox = new QComboBox(settingsPage);
	windowSizeComboBox->setGeometry(136, 441, 236, 40);
	windowSizeComboBox->addItem("1280x720");
	windowSizeComboBox->addItem("640x360");

	backButton = new MenuButton(136, 586, "BACK", settingsPage);
	connect(backButton, &QPushButton::clicked,
			this, &MainMenuPage::onBackButtonClicked);
}

void MainMenuPage::setupAnimations()
{
	triAnim = new QPropertyAnimation(ui->tri, "pos", this);
	triAnim->setDuration(500);
	triAnim->setStartValue(QPoint(0, 0));
	triAnim->setEndValue(QPoint(-767, 0));

	QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
	effect->setOpacity(1.0f);
	pages->setGraphicsEffect(effect);
	fadeOutAnim = new QPropertyAnimation(effect, "opacity");
	fadeOutAnim->setDuration(500);
	fadeOutAnim->setStartValue(0);
	fadeOutAnim->setEndValue(1);
}
