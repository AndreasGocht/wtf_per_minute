#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <chrono>
#include <QTimer>
#include <vector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void update();

    void on_wtfButton_clicked();

    void on_noWtfButton_clicked();

    void on_breakButton_clicked();

private:
    Ui::MainWindow *ui;
    std::vector<std::chrono::system_clock::time_point> wtfs;
    std::chrono::system_clock::time_point start;
    QTimer* timer;

    std::chrono::system_clock::time_point breakStart;

    bool inBreak;
    std::chrono::system_clock::duration breakTotal;
};

#endif // MAINWINDOW_H
