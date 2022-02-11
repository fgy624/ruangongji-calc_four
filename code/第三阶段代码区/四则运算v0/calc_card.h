#ifndef CALC_CARD_H
#define CALC_CARD_H

#include <iostream>
#include <QWidget>
#include <QDebug>
#include <QTime>
#include <QTimer>
#include <QStack>
#include <vector>

namespace Ui {
class Calc_card;
}

class Calc_card : public QWidget
{
    Q_OBJECT

public:
    explicit Calc_card(QWidget *parent = nullptr);
    ~Calc_card();

private slots:
    void on_submit_clicked();
    void timekeeper();
private:
    Ui::Calc_card *ui;

    int time_counter_display=20;
    QTimer *ftime;
    QTime time_counter;

    int num_right=0;
    int num_question=5;

    char operate[4] = { '+','-','*','/' };//后续有待扩展
    int mode;//模式选择****
    long long measure(long long x, long long y)//约分 （bug待修复：2/1=2 0/1=0 date:2022.01.11）
    {
        long long z = y;
        while (x % y != 0)
        {
            z = x % y;
            x = y;
            y = z;
        }
        return z;
    }

    typedef struct node
    {
        int type = 0;//type==0,整数   type==1,分数  type==2,运算符
        int kind = 0;//kind =0 为整数算式 kind=1为分数算式
        int fenzi = 0;
        //type==0,整数的值=分子的值
        //type==1，分数的值=分子的值/分母的值
        int fenmu = 0;
        //只有type==1时该值有意义
        int op = 0;
        //type==2，op==0：+  op==1：-    op==2：*   op==3：/
        //只有type=2时该值有意义

        int flag = 0;//除0异常 flag=1表示除0
        int sign = 0;//叶子节点与非叶子节点，叶子节点=0，非叶子=1|||（new fun）
        int mul = -1;// mul=0 ^ mul =1 **
        int zhishu = 1;//2-4

        struct node* leftchild = NULL;

        struct node* rightchild = NULL;

    }NODE, * Node;

    QStack<NODE> cal;//运算栈
    QVector<NODE*> save;//存放后缀表达式
    NODE* roots[2000];//根节点存放数组
    NODE ANS[2000];//答案数组
    long long int right_counter = 0;//记录正确题数

    void gentree(int a);
    NODE* gen(int depth, int type);
    void expression(NODE* nd);

    QString temp[1010];
    void print(node* nd, int counter);

    bool check(NODE* T_1, NODE* T_2);
    bool checksame(int k);

    NODE calculate();

    bool check_userAnswer(NODE n);
};

#endif // CALC_CARD_H
