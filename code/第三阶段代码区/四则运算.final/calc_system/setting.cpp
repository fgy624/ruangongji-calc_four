#include "setting.h"
#include "ui_setting.h"

int Setting::numq_int=0;
Setting::Setting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Setting)
{
    ui->setupUi(this);
}

Setting::~Setting()
{
    delete ui;
}



void Setting::on_radioButton_clicked()
{
    emit(sign_1());
}

void Setting::on_radioButton_2_clicked()
{
    emit(Setting::sign_2());
}

void Setting::on_confirm_clicked()
{
    QString qnum=ui->lineEdit->text();
    numq_int=qnum.toInt();
    emit(number_q(numq_int));
    this->close();
}
