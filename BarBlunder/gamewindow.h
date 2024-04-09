#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>

namespace Ui {
class GameWindow;
}

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

public slots:
    void on_pushButton_clicked();

signals:
    void backButtonClicked();

private:
    Ui::GameWindow *ui;
};

#endif // GAMEWINDOW_H
