#include<cstdio>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<iostream>
#include<stack>
#include<vector>
#include<limits.h>
using namespace std;
char operate[4] = { '+','-','*','/' };//后续有待扩展
int flag = 0;//标识除0
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

    struct node* leftchild=NULL;

    struct node* rightchild=NULL;

}NODE,*Node;

stack<NODE> cal;//运算栈
vector<NODE*> save;//存放后缀表达式
NODE* roots[2000];//根节点存放数组
NODE ANS[2000];//答案数组

bool check(NODE* T_1, NODE* T_2) //传参根节点
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
            (T_1->fenzi != T_2->fenzi) || (T_1->fenmu != T_2->fenmu)
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
                    (T_1->leftchild->op == T_2->leftchild->op) && (T_1->leftchild->kind == T_2->leftchild->kind))
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

bool checksame(int k)//检查当前根节点与遍历之前的节点之间是否同构
{
    for (int i = 0; i < k; i++)
    {
        if (check(roots[i], roots[k]))//发现同构
        {
            cout << "与roots[" << i << "]同构" << endl;
            return true;
        }
    }
    return false;
}


NODE* gen(int depth, int type)
//op=1是字符，type=0是整数，type=1是分数
{
    NODE* tmp = (NODE*)malloc(sizeof(NODE));

    if (depth <= 0)//叶节点 num
    {
        if (tmp)//2022 1.14 消除warning 对空指针的引用
        {
            tmp->sign = 0;
            tmp->kind = type;
            tmp->leftchild = NULL;
            tmp->rightchild = NULL;
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

void expression(NODE* nd)//生成后缀表达式
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
 
NODE calculate(NODE* nd)//传参为roots[i] 分数计算，整数待接入 
{
    for (unsigned int i = 0; i < save.size(); i++)//2022.01.14warning vector.size为unsigned类型
    {
        if (save[i]->type != 2)//不是op
        {
            cal.push(*save[i]);
        }
        else
        {
            if (save[i]->kind == 0)//整数算式
            {
            	/*有待整合*/ 
            }
            else//分数算式
            {
                NODE a, b;
                a = cal.top();
                cal.pop();
                b = cal.top();
                cal.pop();
                NODE c;
                c.kind = 1;
                c.type = 1;
                c.op = 10000;
                c.fenzi = 0;
                c.fenmu = 0;
                if (save[i]->op == 0)//+
                {
                    c.fenmu = a.fenmu * b.fenmu;
                    c.fenzi = a.fenzi * b.fenmu + a.fenmu * b.fenzi;
                }
                else if (save[i]->op == 1)//-
                {
                    c.fenmu = a.fenmu * b.fenmu;
                    c.fenzi = a.fenmu * b.fenzi - a.fenzi * b.fenmu;
                }
                else if (save[i]->op == 2)//*
                {
                    c.fenzi = b.fenzi * a.fenzi;
                    c.fenmu = a.fenmu * b.fenmu;
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
                        c.fenzi = b.fenzi * a.fenmu;
                        c.fenmu = b.fenmu * a.fenzi;
                    }
                }
                int gongyue = measure(c.fenzi, c.fenmu);
                c.fenzi = c.fenzi / gongyue;
                c.fenmu = c.fenmu / gongyue;
                cal.push(c);
            }
        }
    }
    NODE ans = cal.top();
    return ans;
}

void gentree(int total)
{
    srand((int)time(0));
    for (int i = 0; i < total; i++)
    {
        int depth = rand() % 3 + 2;
        //cout << opnum << "!@!@" << depth << endl;
        int type;
        if (rand() % 2 == 0)
            type = 0;
        else
            type = 1;
        roots[i] = gen(depth, type);

        if (checksame(i) == true)//整合后 
        {
            cout << "当前表达式：";
            print(roots[i]);
            cout << "产生同构" << endl;
            system("PAUSE");
            i--;
            continue;
        }
        else
        {
            expression(roots[i]);
            ANS[i] = calculate(roots[i]);//存储答案
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
                cout << "NO " << i + 1 << ":" << endl;
                //有待接入打印模块和输出答案模块 
                //print(roots[i]); 
                //cout << endl;
                //out(ANS[i]);//输出答案
                while (!save.empty())//清空save
                {
                    save.pop_back();
                }
            }
        }
    }
}
