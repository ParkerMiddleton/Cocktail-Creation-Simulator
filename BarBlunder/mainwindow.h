#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>

// Forward declaration.
class MenuLayer;
class GameLayer;
class ApplicationModel;

class QMediaPlayer;
class QAudioOutput;

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

/// @brief Educational Application main window. Part of View.
class MainWindow : public QMainWindow
{
	Q_OBJECT

friend class LayersEventsFilter;

public:
	/// @brief Constructor.
	/// Initializes the UI and connections.
	MainWindow(ApplicationModel *app, QWidget *parent = nullptr);

	/// @brief Destructor.
	~MainWindow();

public slots:
	/// @brief Stops game music and starts playing menu music.
	void playMenuMusic();

	/// @brief Stops menu music and starts playing game music.
	void playGameMusic();

	/// @brief Enables and disables fullscreen mode.
	/// @param state If true, enables. If false, disables.
	void setFullscreenMode(bool state);

	/// @brief Sets a new size for window mode.
	/// @param newSize New size.
	void setSize(const QSize &newSize);

signals:
	/// @brief Notifies that Escape key on keyboard was pressed.
	void escapeKeyPressed();

protected:
	void resizeEvent(QResizeEvent *event) override;
	void keyPressEvent(QKeyEvent *event) override;

private:
	static constexpr int RENDER_NATIVE_WIDTH = 1280;
	static constexpr int RENDER_NATIVE_HEIGHT = 720;

	Ui::MainWindow *ui;

	QGraphicsView viewport;
	QGraphicsScene viewportScene;

	MenuLayer *menuLayer;
	GameLayer *gameLayer;

	QSize windowSize;

	// Music
	QMediaPlayer *player;
	QAudioOutput *audioOutput;

	/// @brief Helper method to initialize audio fields.
	void setupAudio();

};

#endif // MAINWINDOW_H
