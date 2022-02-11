#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "calc_card.h"
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_begin_button_clicked()
{

    Calc_card *ccd=new Calc_card();
    ccd->show();
    this->close();
}
