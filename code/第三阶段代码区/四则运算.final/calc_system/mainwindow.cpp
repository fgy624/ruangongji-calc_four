#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "calc_card.h"
#include <QWidget>

int MainWindow::Mode=0;
int MainWindow::nq=0;
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

    Calc_card *ccd=new Calc_card(nullptr, Mode, nq);
    ccd->show();
    this->close();
}

void MainWindow::on_setting_clicked()
{
    set=new Setting();
    set->show();
    connect(set,SIGNAL(sign_1()),this,SLOT(get_sign()));
    connect(set,SIGNAL(sign_2()),this,SLOT(get_sign_2()));
    connect(set,SIGNAL(number_q(int)),this,SLOT(get_qnum(int)));
}

void MainWindow::get_sign(){
    MainWindow::Mode=1;
}

void MainWindow::get_sign_2(){
    MainWindow::Mode=0;
}

void MainWindow::get_qnum(int a){
    MainWindow::nq=a;
}
