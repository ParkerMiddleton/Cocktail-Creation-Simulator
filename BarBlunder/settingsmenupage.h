#ifndef SETTINGSMENUPAGE_H
#define SETTINGSMENUPAGE_H

#include <QWidget>

// Forward Declaration.
class MenuLayer;
class ApplicationModel;

QT_BEGIN_NAMESPACE
namespace Ui
{
class SettingsMenu;
}
QT_END_NAMESPACE

/// @brief Settings Menu. Part of View.
class SettingsMenuPage : public QWidget
{
	Q_OBJECT

public:
	/// @brief Constructor.
	/// Initializes the UI and connections.
	/// @param app Model responsible for managing states and settings.
	explicit SettingsMenuPage(ApplicationModel *app, QWidget *parent = nullptr);

	/// @brief Destructor.
	~SettingsMenuPage();

public slots:
	/// @brief Sets the current values of all settings to the given ones.
	/// @param audioVolume Audio volume.
	/// @param fullscreenState Is in fullscreen mode?
	/// @param windowSize Size of the window mode.
	void setCurrentValues(float audioVolume, bool fullscreenState, const QSize &windowSize);

signals:
	/// @brief Notifies the change of value in VolumeSpinBox.
	/// @param volume New volume value, normalized for use in model.
	void volumeSpinBoxValueChanged(float volume);

	/// @brief Notifies the change of check in FullscreenCheckBox.
	/// @param isChecked If check is checked (weird...)
	void fullscreenCheckBoxStateChanged(bool isChecked);

	/// @brief Notifies the change of current index in WindowSizeComboBox.
	/// @param windowSize Size of the window stored at the new selected index.
	void windowSizeComboBoxIndexChanged(const QSize &windowSize);

	/// @brief Notifies that BackButton was clicked.
	void backButtonClicked();

private slots:
	/// @brief Processes the new value at VolumeSpinBox and notifies the model.
	/// @param volume New current value of VolumeSpinBox.
	void checkVolumeSpinBoxValue(int volume);

	/// @brief Processes the new state at FullscreenCheckBox and notifies the model.
	void checkFullscreenCheckBoxState();

	/// @brief Processes the new index at WindowSizeComboBox and notifies the model.
	/// @param index New current index of WindowSizeComboBox.
	void checkWindowSizeComboBoxIndex(int index);

	/// @brief Sends a signal if BackButton was clicked.
	void onBackButtonClicked();

private:
	Ui::SettingsMenu *ui;

};

#endif // SETTINGSMENUPAGE_H
