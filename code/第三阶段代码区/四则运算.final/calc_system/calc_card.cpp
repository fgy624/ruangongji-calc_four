#include "calc_card.h"
#include "ui_calc_card.h"
#include "history.h"
#include "ui_history.h"
#include <QMessageBox>
#include<sys/time.h>

#include <QFile>

//int Calc_card::nqq=0;
Calc_card::Calc_card(QWidget *parent,int md,int nq) :
    QWidget(parent),
    ui(new Ui::Calc_card)
{
    mode=md;
    nqq=nq;
    num_question=nq;

    ui->setupUi(this);
    QDateTime dateTime(QDateTime::currentDateTime());
    TT=dateTime.toString("yy-MM-dd hh:mm::ss ddd");

    QFile file("题库.txt");
    file.open(QIODevice::ReadWrite | QIODevice::Append);
    QTextStream out(&file);
    out<<endl;
    out<<"Time of starting:"<<TT<<endl;

    QFile f("历史记录.txt");
    f.open(QIODevice::ReadWrite | QIODevice::Append);
    QTextStream out1(&f);
    out1<<endl;
    out1<<"Time of starting:"<<TT<<endl;


    gentree(1000);
    ui->question->setText(temp[0]);

    ftime=new QTimer(this);
    ftime->start();
    ftime->setInterval(1000);
    connect(ftime,SIGNAL(timeout()),this,SLOT(timekeeper()));

}


Calc_card::~Calc_card()
{
    delete ui;
}

void Calc_card::gentree(int total)
{
    srand((int)time(0));
    for (int i = 0; i < total; i++)
    {
        int counter=i;
        int depth = rand() % 3 + 2;
        //cout << opnum << "!@!@" << depth << endl;
        int type;
        if (rand() % 2 == 0)
            type = 0;
        else
            type = 1;
        roots[i] = gen(depth, type);

        if (checksame(i) == true)//如果发现重复了，这里改了
        {
            i--;
            continue;
        }
        else
        {
            expression(roots[i]);
            ANS[i] = calculate();//存储答案
            if (ANS[i].flag == 1)//除0操作
            {
                i--;
                while (!save.empty())//清空save
                {
                    save.pop_back();
                }
                continue;
            }
            else
            {
                //qDebug("yyyy");

                QFile file("题库.txt");
                file.open(QIODevice::ReadWrite | QIODevice::Append);
                QTextStream out(&file);
                print(roots[i], counter);
                out << "NO " << i + 1 << ":" << endl << temp[counter] << endl;
                file.close();

                while (!save.empty())//清空save
                {
                    save.pop_back();
                }
            }
        }
    }
}

Calc_card::NODE* Calc_card::gen(int depth, int type)
//op=1是字符，type=0是整数，type=1是分数
{
    NODE* tmp = (NODE*)malloc(sizeof(NODE));
    tmp->mul = -1;
    tmp->zhishu = 1;

    if (depth <= 0)//叶节点 num
    {
        if (tmp)//2022 1.14 消除warning 对空指针的引用
        {
            tmp->sign = 0;
            tmp->kind = type;
            tmp->leftchild = NULL;
            tmp->rightchild = NULL;
            int possibility = rand() % 20;
            if (possibility == 0 || possibility == 19)//乘方1/10概率
            {
                tmp->mul = mode;
                tmp->zhishu = rand() % 3 + 2;
            }
            if (type == 0)//整数
            {
                tmp->type = 0;
                tmp->op = 10000;//打印使用
                tmp->fenzi = rand() % 99 + 1;
                tmp->fenmu = INT_MAX;
            }
            else if (type == 1)//分数
            {

                tmp->type = 1;
                tmp->fenmu = rand() % 18 + 2;
                tmp->op = 10000;//打印使用
               // cout << "sign" << tmp->fenmu << "sign" << endl;
                tmp->fenzi = rand() % tmp->fenmu;
                //cout << "sign1" << tmp->fenzi << "sign1" << endl;
            }
        }
    }
    else//根节点，op
    {
        if (tmp)
        {
            tmp->sign = 1;
            tmp->kind = type;
            tmp->type = 2;//运算符（不包括括号）
            tmp->op = rand() % 4;

            if (rand() % 2 == 0)
                tmp->leftchild = gen(0, type);
            else
                tmp->leftchild = gen(depth - 1, type);
            if (rand() % 2 == 0)
                tmp->rightchild = gen(0, type);
            else
                tmp->rightchild = gen(depth - 1, type);
        }
    }
    return tmp;
}

void Calc_card::expression(NODE* nd)//生成后缀表达式
{
    if (nd->leftchild == NULL || nd->rightchild == NULL)
    {
        save.push_back(nd);
    }
    else
    {
        if (nd->leftchild != NULL)
        {
            expression(nd->leftchild);
        }
        if (nd->rightchild != NULL)
        {
            expression(nd->rightchild);
        }
        save.push_back(nd);
    }
}

void Calc_card::print(node* nd, int counter)//打印表达式
{
    int sign = 0;//每次递归标记是否需要加括号
    if (nd->leftchild == NULL && nd->rightchild == NULL)
    {
        if (nd->mul == -1)
        {
            if (nd->type == 0)
            {
                temp[counter] = temp[counter] + QString::number(nd->fenzi);//存文件准备
            }
            else if (nd->type == 1)
            {
                temp[counter] = temp[counter] + QString::number(nd->fenzi) + "|" + QString::number(nd->fenmu);
            }
        }
        else
        {
            if (nd->type == 0)
            {
                if (mode == 0)
                {
                    temp[counter] = temp[counter] + QString::number(nd->fenzi)+"^";//存文件准备
                }
                else if (mode == 1)
                {
                    temp[counter] = temp[counter] + QString::number(nd->fenzi)+"**";//存文件准备
                }
                temp[counter] = temp[counter] + QString::number(nd->zhishu);//存文件准备
            }
            else if (nd->type == 1)
            {
                if (mode == 0)
                {
                    temp[counter] = temp[counter] + "(" + QString::number(nd->fenzi) + "|" + QString::number(nd->fenmu) + ")^";
                }
                else if (mode == 1)
                {
                    temp[counter] = temp[counter] + "(" + QString::number(nd->fenzi) + "|" + QString::number(nd->fenmu) + ")**";
                }

                temp[counter] = temp[counter] + QString::number(nd->zhishu);
            }

        }

    }
    else
    {
        if (nd->leftchild != NULL)
        {
            if (((nd->op == 0 || nd->op == 1) && (nd->leftchild->op == 2 || nd->leftchild->op == 3 || nd->leftchild->op == 10000)) || ((nd->op == 2 || nd->op == 3) && (nd->leftchild->op == 10000)))
                //加法、减法优先级低于乘法除法且数字op优先级最高（为打印括号准备）
                print(nd->leftchild,counter);
            else
            {
                sign++;
                temp[counter] = temp[counter] + "(";
                print(nd->leftchild,counter);
            }
        }
        //一次递归最多只可能加一层括号
        if (sign > 0)//完成一个子树的遍历后需判断右括号的有无
        {
            temp[counter] = temp[counter] + ")";
            sign--;
        }
        temp[counter] = temp[counter] + operate[nd->op];
        if (nd->rightchild != NULL)
        {
            if (((nd->op == 0 || nd->op == 1) && (nd->rightchild->op == 2 || nd->rightchild->op == 3 || nd->rightchild->op == 10000)) || ((nd->op == 2 || nd->op == 3) && (nd->rightchild->op == 10000)))
                print(nd->rightchild,counter);
            else
            {
                sign++;
                temp[counter] = temp[counter] + "(";
                print(nd->rightchild,counter);
            }
        }
        if (sign > 0)//完成一个子树的遍历后需判断右括号的有无
        {
            temp[counter] = temp[counter] + ")";
            sign--;
        }

    }

}

bool Calc_card::check(NODE* T_1, NODE* T_2) //传参根节点
{
    if (T_1 == nullptr && T_2 == nullptr)
    {//若两棵树均为空，一定同构
        return true;
    }
    else if (T_1 == nullptr || T_2 == nullptr)
    {//如果一棵树不空，另一棵树空，一定不同狗
        return false;
    }
    else
    {//两棵树均不空
        if ((T_1->op != T_2->op) || (T_1->kind != T_2->kind) || (T_1->type != T_2->type) ||
            (T_1->fenzi != T_2->fenzi) || (T_1->fenmu != T_2->fenmu) || (T_1->zhishu != T_2->zhishu)
            )
        {//如果两棵树的当前节点数据不一致，一定不同构(根节点op不同)
            return false;
        }
        else {//节点一致
            if (T_1->leftchild == nullptr && T_2->leftchild == nullptr) {//如果左子树均为空，比较右子树
                return check(T_1->rightchild, T_2->rightchild);
            }
            else if (T_1->leftchild == nullptr || T_2->leftchild == nullptr) {//如果一个左子树为空另一个不为空，必不同构
                return false;
            }
            else {
                if ((T_1->leftchild->type == T_2->leftchild->type) && (T_1->leftchild->fenzi == T_2->leftchild->fenzi) &&
                    (T_1->leftchild->fenmu == T_2->leftchild->fenmu) &&
                    (T_1->leftchild->op == T_2->leftchild->op) && (T_1->leftchild->kind == T_2->leftchild->kind)
                    && (T_1->leftchild->zhishu == T_2->leftchild->zhishu)
                    )
                    //(T_1->leftchild->data == T_2->leftchild->data)
                {//如果左子树均不为空，且值相同，继续判断左右子树
                    return check(T_1->leftchild, T_2->leftchild) && check(T_1->rightchild, T_2->rightchild);
                }
                else
                {//如果左子树的值不同，且此时为 “ + ” 运算符或者 “ * ” 运算符，交换左右子树看是否同构
                    if (T_1->op == 0 || T_2->op == 2) {
                        return check(T_1->leftchild, T_2->rightchild) && check(T_1->rightchild, T_2->leftchild);
                    }
                    else {//否则，不同构
                        return false;
                    }
                }
            }
        }
    }
    //cout << "hello";
}

bool Calc_card::checksame(int k)//检查当前根节点与遍历之前的节点之间是否同构
{
    for (int i = 0; i < k; i++)
    {
        if (check(roots[i], roots[k]))//发现同构
        {
            //cout << "与roots[" << i << "]同构" << endl;
            return true;
        }
    }
    return false;
}

Calc_card::NODE Calc_card::calculate()//传参为roots[i]
{
    for (int i = 0; i < save.size(); i++)//2022.01.14warning vector.size为unsigned类型
    {
        if (save[i]->type != 2)//不是op
        {
            cal.push(*save[i]);
        }
        else
        {
            if (save[i]->kind == 0)//整数算式
            {
                NODE a, b;
                a = cal.top();
                cal.pop();
                b = cal.top();
                cal.pop();
                NODE c;
                c.zhishu = 1;
                c.kind = 0;
                c.type = 0;
                c.op = 10000;
                c.fenzi = 0;
                c.fenmu = 0;
                if (save[i]->op == 0)//+
                    c.fenzi = pow(a.fenzi, a.zhishu) + pow(b.fenzi, b.zhishu);
                else if (save[i]->op == 1)//-
                    c.fenzi = pow(b.fenzi, b.zhishu) - pow(a.fenzi, a.zhishu);
                else if (save[i]->op == 2)//*
                    c.fenzi = pow(b.fenzi, b.zhishu) * pow(a.fenzi, a.zhishu);

                else// /
                {
                    if (a.fenzi == 0)
                    {
                        c.flag = 1;
                        return c;
                        //error;
                    }
                    else
                    {
                        c.fenzi = pow(b.fenzi, b.zhishu) / pow(a.fenzi, a.zhishu);
                    }
                }
                cal.push(c);
            }
            else//分数算式
            {
                NODE a, b;
                a = cal.top();
                cal.pop();
                b = cal.top();
                cal.pop();
                NODE c;
                c.zhishu = 1;
                c.kind = 1;
                c.type = 1;
                c.op = 10000;
                c.fenzi = 0;
                c.fenmu = 0;
                if (save[i]->op == 0)//+
                {
                    c.fenmu = pow(a.fenmu, a.zhishu) * pow(b.fenmu, b.zhishu);
                    c.fenzi = pow(a.fenzi, a.zhishu) * pow(b.fenmu, b.zhishu) + pow(a.fenmu, a.zhishu) * pow(b.fenzi, b.zhishu);
                }
                else if (save[i]->op == 1)//-
                {
                    c.fenmu = pow(a.fenmu, a.zhishu) * pow(b.fenmu, b.zhishu);
                    c.fenzi = pow(a.fenmu, a.zhishu) * pow(b.fenzi, b.zhishu) - pow(a.fenzi, a.zhishu) * pow(b.fenmu, b.zhishu);
                }
                else if (save[i]->op == 2)//*
                {
                    c.fenzi = pow(b.fenzi, b.zhishu) * pow(a.fenzi, a.zhishu);
                    c.fenmu = pow(a.fenmu, a.zhishu) * pow(b.fenmu, b.zhishu);
                }
                else// /
                {
                    if (a.fenzi == 0)
                    {
                        c.flag = 1;
                        return c;
                        //error;
                    }
                    else
                    {
                        c.fenzi = pow(b.fenzi, b.zhishu) * pow(a.fenmu, a.zhishu);
                        c.fenmu = pow(b.fenmu, b.zhishu) * pow(a.fenzi, a.zhishu);
                    }
                }
                long long int gongyue = measure(c.fenzi, c.fenmu);
                c.fenzi = c.fenzi / gongyue;
                c.fenmu = c.fenmu / gongyue;
                cal.push(c);
            }
        }
    }
    NODE ans = cal.top();
    return ans;
}

//判断用户输入是否正确
bool Calc_card::check_userAnswer(NODE n) {

    //用户输入
    QString user_answer=ui->answer_line->text();

    //处理正确答案
    QString ans_right;
    if (n.type == 0) {//正确答案为整数
        ans_right = QString::number(n.fenzi);
    }
    else {//正确答案为分数
        QString ans_right_fenzi;
        QString ans_right_fenmu;
        if(n.fenzi==0)
        {
            if(user_answer=="0")
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        if(n.fenzi*n.fenmu<0)
        {
            if(n.fenmu<0)
            {//分子分母分别处理为字符串
                ans_right_fenzi = "-"+QString::number(n.fenzi);
                ans_right_fenmu = QString::number(-n.fenmu);
            }
            else
            {
                ans_right_fenzi = QString::number(n.fenzi);
                ans_right_fenmu = QString::number(n.fenmu);
            }
        }
        else
        {
            if(n.fenmu<0)
            {
                ans_right_fenzi = QString::number(-n.fenzi);
                ans_right_fenmu = QString::number(-n.fenmu);
            }
            else
            {
                ans_right_fenzi = QString::number(n.fenzi);
                ans_right_fenmu = QString::number(n.fenmu);
            }
        }


        //拼接为一整个字符串
        ans_right = ans_right_fenzi + "|" + ans_right_fenmu;
    }

    //比较部分
    if (user_answer != ans_right) {
        return false;
    }
    else {
        return true;
    }
}

void Calc_card::timekeeper(){
    ui->clock->display(Calc_card::time_counter_display);
    Calc_card::time_counter_display--;
    if(time_counter_display<0){
        num_question--;
        QMessageBox info(QMessageBox::Warning,"结果","您已超时");
        info.exec();

        if(num_question <= 0){
            QMessageBox msgBox;
            msgBox.setText("您已答完所有题，是否查看错题报告？");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No );

            int ret = msgBox.exec();
            switch (ret) {
            case QMessageBox::Yes:
            {
                history *his=new history;
                his->show();
                this->close();
                break;
            }
            case QMessageBox::No:
                this->close();
                break;
            }
        }
        else{
            ui->question->setText(temp[nqq-num_question]);
            time_counter_display=20;
        }
    }
}

void Calc_card::on_submit_clicked()
{
    QFile f("历史记录.txt");
    f.open(QIODevice::ReadWrite | QIODevice::Append);
    QTextStream out(&f);
    ftime->stop();

    QString answer=ui->answer_line->text();

    if(answer==NULL){
        QMessageBox answer_warning(QMessageBox::Warning,"警告","答案为空");
        answer_warning.exec();
        ftime->start();
    }
    else{
        num_question--;
        bool answer_result=check_userAnswer(ANS[nqq-num_question-1]);
        //check(answer)
        if(answer_result==false)
        {
            out<<"NO"<<nqq-num_question-1<<": "<<endl;
            out<<temp[nqq-num_question-1]<<" is wrong"<<endl;
            out<<"Your answer is: "<<ui->answer_line->text()<<endl;
            if (ANS[nqq-num_question-1].kind == 0)
            {
                out<<"The correct answer: "<<ANS[nqq-num_question-1].fenzi<<endl;
            }
            else
            {
                if(ANS[nqq-num_question-1].fenzi==0)
                {
                    out<<"The correct answer: 0"<<endl;
                }
                else if(ANS[nqq-num_question-1].fenmu==1)
                {
                    out<<"The correct answer: "<<ANS[nqq-num_question-1].fenmu<<endl;
                }
                else
                {
                    out<<"The correct answer: "<<ANS[nqq-num_question-1].fenzi<<"|"<<ANS[nqq-num_question-1].fenmu<<endl;
                }
            }
            QMessageBox result(QMessageBox::Warning,"结果","您的答案错了");
            result.exec();

        }
        else
        {
            out<<"NO"<<nqq-num_question-1<<": "<<endl;
            out<<temp[nqq-num_question-1]<<" is correct"<<endl;
            out<<"Your answer is: "<<ui->answer_line->text()<<endl;
            if (ANS[nqq-num_question-1].kind == 0)
            {
                out<<"The correct answer:  "<<ANS[nqq-num_question-1].fenzi<<endl;
            }
            else
            {
                if(ANS[nqq-num_question-1].fenzi==0)
                {
                    out<<"The correct answer:  0"<<endl;
                }
                else if(ANS[nqq-num_question-1].fenmu==1)
                {
                    out<<"The correct answer:  "<<ANS[nqq-num_question-1].fenmu<<endl;
                }
                else
                {
                    out<<"The correct answer:   "<<ANS[nqq-num_question-1].fenzi<<"|"<<ANS[nqq-num_question-1].fenmu<<endl;
                }
            }
            QMessageBox result(QMessageBox::Warning,"结果","您的答案对了");
            result.exec();
            Calc_card::num_right++;

            ui->score->setText(QString::number(num_right));
        }
        f.close();
        if(num_question <= 0){
            QMessageBox msgBox;
            msgBox.setText("您已答完所有题，是否查看错题报告？");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No );

            int ret = msgBox.exec();
            switch (ret) {
            case QMessageBox::Yes:
            {
                history *his=new history;
                his->show();
                this->close();
                break;
            }
            case QMessageBox::No:
                this->close();
                break;
            }

        }
        else{
            ui->answer_line->clear();
            ui->question->setText(temp[nqq-num_question]);
            time_counter_display=20;
            ftime->start();
        }
    }


}
