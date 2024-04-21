#include "recipenote.h"

#include <QVBoxLayout>
#include <QScrollBar>
#include <QFont>

QFont defaultFont;
QFont strikedOutFont;

RecipeStep::RecipeStep(const QString &instruction, QWidget *parent)
	: QWidget{parent}
	, layout{this}
{
	int iconSize = 25;
	this->setFixedHeight(iconSize);

	iconLabel.setFixedWidth(iconSize);
	iconLabel.setFixedHeight(iconSize);
	textLabel.setFont(defaultFont);

	textLabel.setFixedHeight(iconSize);
	textLabel.setText(instruction);
	textLabel.setFont(defaultFont);

	this->setStyleSheet("background-color: transparent; color: black; padding: 4px;"); //border: 0px solid #444444

	layout.addRow(&iconLabel, &textLabel);

	layout.setContentsMargins(0, 0, 0, 0);
	layout.setSpacing(0);
	this->setLayout(&layout);
}

void RecipeStep::setIsCorrect(bool state)
{
	if (state)
	{
		iconLabel.setText("");
		textLabel.setFont(strikedOutFont);
	}
	else
	{
		iconLabel.setText("X");
	}
}

RecipeNote::RecipeNote(QWidget *parent)
	: QScrollArea{parent}
{
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->horizontalScrollBar()->setEnabled(false);
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	defaultFont.setFamily("Segoe Print");
	defaultFont.setBold(true);
	defaultFont.setPointSize(10);
	strikedOutFont = defaultFont;
	strikedOutFont.setStrikeOut(true);
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
	// +1 since the first one is drink's name.
	steps[stepNumber + 1]->setIsCorrect(true);
}

void RecipeNote::showStepIsIncorrect(int stepNumber)
{
	// +1 since the first one is drink's name.
	steps[stepNumber + 1]->setIsCorrect(false);
}
