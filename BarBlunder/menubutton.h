#ifndef MENUBUTTON_H
#define MENUBUTTON_H

class MenuButton : public QPushButton
{
	Q_OBJECT

public:
	MenuButton(int x, int y, const QString &text, QWidget *parent);

};

#endif // MENUBUTTON_H
