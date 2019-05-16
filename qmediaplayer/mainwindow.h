#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void saveScreenshot();
    void shootScreen();

private:
    Ui::MainWindow *ui;

    QPixmap originalPixmap;
    QLabel *screenshotLabel;
    QPushButton *newScreenshotButton;


};

#endif // MAINWINDOW_H


