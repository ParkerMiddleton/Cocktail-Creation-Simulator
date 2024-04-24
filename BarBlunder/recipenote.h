#ifndef RECIPENOTE_H
#define RECIPENOTE_H

#include <QWidget>
#include <QScrollArea>
#include <QString>
#include <QList>
#include <QLabel>
#include <QFormLayout>

class QVBoxLayout;

/// @brief An entry displaying a step information inside the RecipeNote.
/// @extends QWidget
class RecipeStep : public QWidget
{
	Q_OBJECT

public:
	/// @brief Constructor.
	/// @param instruction Step information text.
	explicit RecipeStep(const QString &instruction, QWidget *parent = nullptr);

	/// @brief Changes the appearance of the step depending on the given value.
	/// @param state Is it correct or not?
	void setIsCorrect(bool state);

private:
	QFormLayout layout;
	QLabel iconLabel;
	QLabel textLabel;

};

/// @brief Displays recipe steps. Part of View.
/// @extends QScrollArea
class RecipeNote : public QScrollArea
{
	Q_OBJECT

public:
	/// @brief Constructor.
	/// @param Parent - a address of a QWidget that is a parent defaul ti s anul pointer
	explicit RecipeNote(QWidget *parent = nullptr);

	/// @brief Destructor.
	~RecipeNote();

	/// @brief Set ups the layout of this widget. Must be called manually, since the widget itself is created by QT's UI.
	void setupLayout(QWidget *contents);

public slots:
	/// @brief Updates the displayed recipe steps.
	/// @param recipeSteps Recipe.
	void updateRecipe(const QList<QString> &recipeSteps);

	/// @brief Changes the appearance of the given step to show that it is correct.
	/// @param stepNumber Number of step.
	void showStepIsCorrect(int stepNumber);

	/// @brief Changes the appearance of the given step to show that it is incorrect.
	/// @param stepNumber Number of step.
	void showStepIsIncorrect(int stepNumber);

private:
	QVBoxLayout *contentsLayout;
	QList<RecipeStep*> steps;

};

#endif // RECIPENOTE_H
