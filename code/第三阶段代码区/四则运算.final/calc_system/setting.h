#ifndef SETTING_H
#define SETTING_H

#include <QWidget>

namespace Ui {
class Setting;
}

class Setting : public QWidget
{
    Q_OBJECT

public:
    explicit Setting(QWidget *parent = nullptr);
    ~Setting();

private slots:
    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_confirm_clicked();

private:
    Ui::Setting *ui;
    static int mode;
    static int numq_int;

signals:
    void sign_1();//**
    void sign_2();//^
    void number_q(int num);//题数

};

#endif // SETTING_H
