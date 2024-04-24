#include "recipenote.h"

#include <QVBoxLayout>
#include <QScrollBar>
#include <QFont>

// Global within this translation unit.
QFont defaultFont;
QFont strikedOutFont;
QString defaultSS;
QString incorrectSS;

RecipeStep::RecipeStep(const QString &instruction, QWidget *parent)
	: QWidget{parent}
	, layout{this}
{
	int iconSize = 25;
	this->setFixedHeight(iconSize);
	this->setStyleSheet("background-color: transparent; padding: 4px;");

	iconLabel.setFixedWidth(iconSize);
	iconLabel.setFixedHeight(iconSize);
	iconLabel.setFont(defaultFont);
	iconLabel.setStyleSheet(defaultSS);

	textLabel.setFixedHeight(iconSize);
	textLabel.setText(instruction);
	textLabel.setFont(defaultFont);
	textLabel.setStyleSheet(defaultSS);

	layout.addRow(&iconLabel, &textLabel);

	layout.setContentsMargins(0, 0, 0, 0);
	layout.setSpacing(0);
	this->setLayout(&layout);
}

void RecipeStep::setIsCorrect(bool state)
{
	if (state)
	{
		iconLabel.setStyleSheet(defaultSS);
		textLabel.setStyleSheet(defaultSS);

		iconLabel.setText("");
		textLabel.setFont(strikedOutFont);
	}
	else
	{
		iconLabel.setStyleSheet(incorrectSS);
		textLabel.setStyleSheet(incorrectSS);

		iconLabel.setText("X");
	}
}

RecipeNote::RecipeNote(QWidget *parent)
	: QScrollArea{parent}
{
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->horizontalScrollBar()->setEnabled(false);
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	// Initalize global variables used by RecipeStep.
	defaultFont.setFamily("Segoe Print");
	defaultFont.setBold(true);
	defaultFont.setPointSize(10);
	strikedOutFont = defaultFont;
	strikedOutFont.setStrikeOut(true);

	defaultSS = "color: #2e2e2e;";
	incorrectSS = "color: red;";
}

RecipeNote::~RecipeNote()
{
	for (RecipeStep *entry : steps)
	{
		contentsLayout->removeWidget(entry);
		delete entry;
	}

	steps.clear();
}

void RecipeNote::setupLayout(QWidget *contents)
{
	contentsLayout = new QVBoxLayout(contents);
	contentsLayout->setSpacing(0);
	contentsLayout->setContentsMargins(0, 0, 0, 0);
	this->setWidget(contents);
	contentsLayout->setAlignment(Qt::AlignTop);
	contentsLayout->setSizeConstraint(QLayout::SetMinimumSize);
}

void RecipeNote::updateRecipe(const QList<QString> &recipeSteps)
{
	for (RecipeStep *entry : steps)
	{
		contentsLayout->removeWidget(entry);
		delete entry;
	}

	steps.clear();

	for (const QString &text : recipeSteps)
	{
		RecipeStep *entry = new RecipeStep(text);
		steps.push_back(entry);
		contentsLayout->addWidget(entry);
	}
}

void RecipeNote::showStepIsCorrect(int stepNumber)
{
	steps[stepNumber]->setIsCorrect(true);
}

void RecipeNote::showStepIsIncorrect(int stepNumber)
{
	steps[stepNumber]->setIsCorrect(false);
}
