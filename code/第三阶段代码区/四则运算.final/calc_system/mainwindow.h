#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "calc_card.h"
#include "setting.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private slots:
    void on_begin_button_clicked();

    void on_setting_clicked();

    //**
    void get_sign();
    //^
    void get_sign_2();
    //题数
    void get_qnum(int a);

private:
    Ui::MainWindow *ui;
    Calc_card *ccd;
    Setting *set;

    static int Mode;
    static int nq;
};
#endif // MAINWINDOW_H
