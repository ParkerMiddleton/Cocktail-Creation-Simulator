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

	/// @brief Loads settings and emits signals to the view.
	/// Must be called after the constructor due to QT limitations.
	void initialize();

	/// @brief Returns a pointer to the bar model.
	/// @return BarModel*
	BarModel* barModel();

public slots:
	/// @brief Starts a new game and emits appropriate signal.
	void startNewGame();

	/// @brief Pauses the game and emits appropriate signal.
	void pause();

	/// @brief Unpauses the game and emits appropriate signal.
	void unpause();

	/// @brief Switches the pause state of the game.
	void switchPauseState();

	/// @brief Emits a signal to close the application.
	void exitApplication();

	/// @brief Sets the audio volume setting.
	/// @param volume Setting new value.
	void setAudioVolume(float volume);

	/// @brief Sets the fullscreen boolean state setting.
	/// @param state Setting new value.
	void setIsFullscreen(bool state);

	/// @brief Sets the window size setting.
	/// @param size Setting new value.
	void setWindowSize(const QSize &size);

signals:
	/// @brief Notifies that the settings were loaded and initialized.
	/// @param audioVolume Setting value.
	/// @param fullscreenState Setting value.
	/// @param windowSize Setting value.
	void settingsLoaded(float audioVolume, bool fullscreenState, const QSize &windowSize);

	/// @brief Notifies that the new game was started.
	void newGameStarted();

	/// @brief Notifies that the game was paused.
	void gamePaused();

	/// @brief Notifies that the game was unpaused.
	void gameUnpaused();

	/// @brief Notifies that the application should be closed.
	void applicationExitRequested();

	/// @brief Notifies that the volume setting was changed.
	/// @param newVolume Setting value.
	void audioVolumeChanged(float newVolume);

	/// @brief Notifies that the fullscreen boolean setting was changed.
	/// @param isFullscreen Setting value.
	void fullscreenModeChanged(bool isFullscreen);

	/// @brief Notifies that the window size setting was changed.
	/// @param newSize Setting value.
	void windowSizeChanged(const QSize &newSize);

private:
	BarModel bar;

	enum class State { NotStarted, Paused, Unpaused };
	State currentState;

	// Settings Vars
	float audioVolume;
	bool isFullscreen;
	QSize windowSize;

	/// @brief Helper for saving settings into JSON formatted file.
	/// If unable to open/create file, simply returns.
	void saveSettings();

	/// @brief Helper for loading settings from JSON formatted file.
	/// If unable to open file, simply returns.
	void loadSettings();

};

#endif // APPLICATIONMODEL_H
