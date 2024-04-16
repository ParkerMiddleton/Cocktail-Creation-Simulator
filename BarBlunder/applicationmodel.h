#ifndef APPLICATIONMODEL_H
#define APPLICATIONMODEL_H

#include "barmodel.h"

#include <QObject>
#include <QSize>

/// @brief Responsible for managing states and settings. Part of Model.
class ApplicationModel : public QObject
{
	Q_OBJECT

public:
	/// @brief Constructor.
	explicit ApplicationModel(QObject *parent = nullptr);

	void initialize();
	BarModel* barModel();

public slots:
	void startNewGame();
	void pause();
	void unpause();
	void switchPauseState();
	void exitApplication();

	void setAudioVolume(float volume);
	void setIsFullscreen(bool state);
	void setWindowSize(const QSize &size);

signals:
	void settingsLoaded(float audioVolume, bool fullscreenState, const QSize &windowSize);
	void gameStarted();

	void gamePaused();
	void gameUnpaused();

	void applicationExitRequested();

	void audioVolumeChanged(float newVolume);
	void fullscreenModeChanged(bool isFullscreen);
	void windowSizeChanged(const QSize &newSize);

private:
	BarModel bar;

	enum class State { NotStarted, Paused, Unpaused };
	State currentState;

	// Settings Vars
	float audioVolume;
	bool isFullscreen;
	QSize windowSize;

	void saveSettings();
	void loadSettings();

};

#endif // APPLICATIONMODEL_H
