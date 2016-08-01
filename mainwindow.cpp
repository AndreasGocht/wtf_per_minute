#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    inBreak(0),
    breakTotal(std::chrono::milliseconds(0))
{
    ui->setupUi(this);
    start = std::chrono::system_clock::now();
    wtfs = 0;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(100);
}

MainWindow::~MainWindow()
{
    disconnect(timer, SIGNAL(timeout()), this, SLOT(update()));
    delete timer;
    delete ui;
}

void MainWindow::update()
{
    auto now = std::chrono::system_clock::now();
    auto totalTime = now - start;
    auto realTime = totalTime - breakTotal;
    auto wtf_per_min =
            static_cast<double>(wtfs)/(std::chrono::duration_cast<std::chrono::milliseconds>(realTime).count()) * 1e3 * 60;
    ui->label->setText(QString::number(wtf_per_min, 'f', 2));
}


void MainWindow::on_wtfButton_clicked()
{
    wtfs ++;
}

void MainWindow::on_noWtfButton_clicked()
{
    wtfs --;
}

void MainWindow::on_breakButton_clicked()
{
    if (inBreak)
    {
        inBreak = false;
        ui->breakButton->setText(QString("Start break :-)"));
        auto now = std::chrono::system_clock::now();
        auto timeForThisBreak = now - breakStart;
        breakTotal = breakTotal + timeForThisBreak;
        timer->start();
    }
    else
    {
        inBreak = true;
        breakStart = std::chrono::system_clock::now();
        ui->breakButton->setText(QString("End break :-("));
        timer->stop();
    }
}
