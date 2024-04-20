#include "applicationmodel.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

ApplicationModel::ApplicationModel(QObject *parent)
	: QObject{parent}
	, currentState{State::NotStarted}
	, audioVolume{0.0f}
	, isFullscreen{false}
	, windowSize{1280, 720}
{

}

void ApplicationModel::initialize()
{
	this->loadSettings();
	emit settingsLoaded(audioVolume, isFullscreen, windowSize);
	emit audioVolumeChanged(audioVolume);
	emit windowSizeChanged(windowSize);
	emit fullscreenModeChanged(isFullscreen);

	// ! README !
	// If you want to skip the menu when launching, uncomment the line below!
	//this->startNewGame();
}

BarModel* ApplicationModel::barModel()
{
	return &bar;
}

void ApplicationModel::startNewGame()
{
	bar.startNewGame();
	currentState = State::Unpaused;
	bar.setIsPaused(false);

	emit newGameStarted();
}

void ApplicationModel::pause()
{
	currentState = State::Paused;
	bar.setIsPaused(true);

	emit gamePaused();
}

void ApplicationModel::unpause()
{
	currentState = State::Unpaused;
	bar.setIsPaused(false);

	emit gameUnpaused();
}

void ApplicationModel::switchPauseState()
{
	switch (currentState)
	{
	case State::Unpaused:
		this->pause();
		break;
	case State::Paused:
		this->unpause();
		break;
	default:
		break;
	}
}

void ApplicationModel::exitApplication()
{
	emit applicationExitRequested();
}

void ApplicationModel::setAudioVolume(float volume)
{
	audioVolume = volume;
	this->saveSettings();
	emit audioVolumeChanged(audioVolume);
}

void ApplicationModel::setIsFullscreen(bool state)
{
	isFullscreen = state;
	this->saveSettings();
	emit fullscreenModeChanged(isFullscreen);
}

void ApplicationModel::setWindowSize(const QSize &size)
{
	windowSize = size;
	this->saveSettings();
	emit windowSizeChanged(windowSize);
}

void ApplicationModel::saveSettings()
{
	QFile settingsFile("barblunder_settings.json");

	if (!settingsFile.open(QIODevice::WriteOnly))
		return;

	QJsonObject json;
	json["audio_volume"] = audioVolume;
	json["fullscreen"] = isFullscreen;
	json["window_width"] = windowSize.width();
	json["window_height"] = windowSize.height();

	settingsFile.write(QJsonDocument(json).toJson());
}

void ApplicationModel::loadSettings()
{
	QFile settingsFile("barblunder_settings.json");

	if (!settingsFile.open(QIODevice::ReadOnly))
		return;

	QByteArray jsonData = settingsFile.readAll();
	QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonData));
	const QJsonObject &json = jsonDoc.object();

	if (const QJsonValue &val = json["audio_volume"]; val.isDouble())
	{
		float audio_volume = val.toDouble();
		if (audio_volume >= 0.0f && audio_volume <= 1.0f)
		{
			audioVolume = audio_volume;
		}
	}

	if (const QJsonValue &val = json["fullscreen"]; val.isBool())
	{
		isFullscreen = val.toBool();
	}

	const QJsonValue &valWidth = json["window_width"];
	const QJsonValue &valHeight = json["window_height"];

	if (valWidth.isDouble() && valHeight.isDouble())
	{
		int window_width = valWidth.toInt();
		int window_height = valHeight.toInt();

		if ((window_width == 1280 && window_height == 720) || (window_width == 640 && window_height == 360))
		{
			windowSize.setWidth(window_width);
			windowSize.setHeight(window_height);
		}
	}
}
