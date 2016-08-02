#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <algorithm>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    inBreak(0),
    breakTotal(std::chrono::milliseconds(0))
{
    ui->setupUi(this);
    start = std::chrono::system_clock::now();
    wtfs.clear();
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
    auto wtfPerMin =
            static_cast<double>(wtfs.size())/(std::chrono::duration_cast<std::chrono::milliseconds>(realTime).count()) * 1e3 * 60;

    auto wtfsLastHourFirst = std::lower_bound(wtfs.begin(), wtfs.end(), std::chrono::system_clock::now() - std::chrono::hours(1) );
    auto wtfsLastHour = std::distance(wtfsLastHourFirst, wtfs.end());
    auto wtfsPerMinLastHour = static_cast<double>(wtfsLastHour)/60;

    auto wtfsLast5MinFirst = std::lower_bound(wtfs.begin(), wtfs.end(), std::chrono::system_clock::now() - std::chrono::minutes(5) );
    auto wtfsLast5Min = std::distance(wtfsLast5MinFirst, wtfs.end());
    auto wtfsPerMinLast5Min = static_cast<double>(wtfsLast5Min)/5;

    ui->wtfPerMinLabel->setText(QString("Todays average:\t") + QString::number(wtfPerMin, 'f', 2));

    ui->label->setText(QString("Todays total:\t") + QString::number(wtfs.size(), 'f', 0)
                       + QString("\nLast 5 min:\t\t") + QString::number(wtfsPerMinLast5Min, 'f', 2)
                       + QString("\nLast hour:\t\t") + QString::number(wtfsPerMinLastHour, 'f', 2) );
}


void MainWindow::on_wtfButton_clicked()
{
    wtfs.push_back(std::chrono::system_clock::now());
}

void MainWindow::on_noWtfButton_clicked()
{
    if( wtfs.size() > 0 )
    {
        wtfs.pop_back();
    }
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
