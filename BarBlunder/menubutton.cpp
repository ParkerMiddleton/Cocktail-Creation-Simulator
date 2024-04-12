#include "menubutton.h"

MenuButton::MenuButton(int x, int y, const QString &text, QWidget *parent)
	: QPushButton{parent}
{
	this->setGeometry(x, y, 236, 40);
	this->setText(text);
	this->setStyleSheet("QPushButton {"\
						"background-color: transparent;"\
						"border: none;"\
						"color: #E2E2E2;"\
						"text-align:left;"\
						"font: bold \"Gabriola\";"\
						"font-size: 22px;"\
						"}"\

						"QPushButton:pressed {"\
						"color: red;"\
						"}"\

						"QPushButton::hover {"\
						"color: #FFE770;"\
						"}"\

						"QPushButton::disabled {"\
						"color: #E2E2E2;"\
						"}");
}
