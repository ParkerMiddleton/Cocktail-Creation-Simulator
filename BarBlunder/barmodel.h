#ifndef BARMODEL_H
#define BARMODEL_H

#include <QObject>

/// @brief Educational Application main model class.
class BarModel : public QObject
{
	Q_OBJECT
public:
	/// @brief Constructor.
	explicit BarModel(QObject *parent = nullptr);

	/// @brief Used to emit any signals to the view before the application is fully started, if needed.
	/// Reason why it exists:
	/// according to the official QT documentation and various online sources,
	/// any signal emitted inside the constructor does not go to its respective slot.
	void initialize();

public slots:

signals:
	/// @brief When model is initialized.
	void barOpened();

private:

};

#endif // BARMODEL_H
