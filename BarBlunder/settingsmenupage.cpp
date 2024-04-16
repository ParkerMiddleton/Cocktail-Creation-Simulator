#include "settingsmenupage.h"
#include "ui_settingsmenupage.h"

#include "applicationmodel.h"

SettingsMenuPage::SettingsMenuPage(QWidget *parent)
	: QWidget{parent}
	, ui{new Ui::SettingsMenu}
{
	ui->setupUi(this);

	ui->WindowSizeComboBox->addItem("1280x720");
	ui->WindowSizeComboBox->addItem("640x360");

	connect(ui->VolumeSpinBox, &QSpinBox::valueChanged,
			this, &SettingsMenuPage::checkVolumeSpinBoxValue);

	connect(ui->FullscreenCheckBox, &QCheckBox::clicked,
			this, &SettingsMenuPage::checkFullscreenCheckBoxState);

	connect(ui->WindowSizeComboBox, &QComboBox::currentIndexChanged,
			this, &SettingsMenuPage::checkWindowSizeComboBoxIndex);

	connect(ui->BackButton, &QPushButton::clicked,
			this, &SettingsMenuPage::onBackButtonClicked);
}

SettingsMenuPage::~SettingsMenuPage()
{
	delete ui;
}

void SettingsMenuPage::setCurrentValues(float audioVolume, bool fullscreenState, const QSize &windowSize)
{
	// Volume.
	int value = audioVolume * 10.0f;
	ui->VolumeSpinBox->setValue(value);

	// Fullscreen.
	if (fullscreenState)
	{
		ui->FullscreenCheckBox->setCheckState(Qt::Checked);

		// No need to change window size when in fullscreen.
		ui->WindowSizeLabel->setDisabled(true);
		ui->WindowSizeComboBox->setDisabled(true);
	}
	else
	{
		ui->FullscreenCheckBox->setCheckState(Qt::Unchecked);

		ui->WindowSizeLabel->setDisabled(false);
		ui->WindowSizeComboBox->setDisabled(false);
	}

	// Window size.
	if (windowSize.width() == 1280)
	{
		ui->WindowSizeComboBox->setCurrentIndex(0);
	}
	else if (windowSize.width() == 640)
	{
		ui->WindowSizeComboBox->setCurrentIndex(1);
	}
}

void SettingsMenuPage::setupModelConnections(ApplicationModel *app)
{
	connect(app, &ApplicationModel::settingsLoaded,
			this, &SettingsMenuPage::setCurrentValues);

	connect(this, &SettingsMenuPage::volumeSpinBoxValueChanged,
			app, &ApplicationModel::setAudioVolume);

	connect(this, &SettingsMenuPage::fullscreenCheckBoxStateChanged,
			app, &ApplicationModel::setIsFullscreen);

	connect(this, &SettingsMenuPage::windowSizeComboBoxIndexChanged,
			app, &ApplicationModel::setWindowSize);
}

void SettingsMenuPage::checkVolumeSpinBoxValue(int volume)
{
	float newVolume = (float)volume / 10.0;

	emit volumeSpinBoxValueChanged(newVolume);
}

void SettingsMenuPage::checkFullscreenCheckBoxState()
{
	if (ui->FullscreenCheckBox->checkState() == Qt::CheckState::Checked)
	{
		// No need to change window size when in fullscreen.
		ui->WindowSizeLabel->setDisabled(true);
		ui->WindowSizeComboBox->setDisabled(true);

		emit fullscreenCheckBoxStateChanged(true);
	}
	else if (ui->FullscreenCheckBox->checkState() == Qt::CheckState::Unchecked)
	{
		ui->WindowSizeLabel->setDisabled(false);
		ui->WindowSizeComboBox->setDisabled(false);

		emit fullscreenCheckBoxStateChanged(false);
	}
}

void SettingsMenuPage::checkWindowSizeComboBoxIndex(int index)
{
	if (index == 0)
	{
		emit windowSizeComboBoxIndexChanged({1280, 720});
	}
	else if (index == 1)
	{
		emit windowSizeComboBoxIndexChanged({640, 360});
	}
}

void SettingsMenuPage::onBackButtonClicked()
{
	emit backButtonClicked();
}
