#include<cstdio>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<iostream>
#include<stack>
#include<vector>
#include<limits.h>

#include<string>
using namespace std;
char operate[4] = { '+','-','*','/' };//�����д���չ
int flag = 0;//��ʶ��0
long long measure(long long x, long long y)//Լ�� ��bug���޸���2/1=2 0/1=0 date:2022.01.11��
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
    int type = 0;//type==0,����   type==1,����  type==2,�����
    int kind = 0;//kind =0 Ϊ������ʽ kind=1Ϊ������ʽ
    int fenzi = 0;
    //type==0,������ֵ=���ӵ�ֵ
    //type==1��������ֵ=���ӵ�ֵ/��ĸ��ֵ
    int fenmu = 0;
    //ֻ��type==1ʱ��ֵ������
    int op = 0;
    //type==2��op==0��+  op==1��-    op==2��*   op==3��/
    //ֻ��type=2ʱ��ֵ������

    int flag = 0;//��0�쳣 flag=1��ʾ��0 
    int sign = 0;//Ҷ�ӽڵ����Ҷ�ӽڵ㣬Ҷ�ӽڵ�=0����Ҷ��=1|||��new fun��

    struct node* leftchild=NULL;

    struct node* rightchild=NULL;

}NODE,*Node;

stack<NODE> cal;//����ջ
vector<NODE*> save;//��ź�׺���ʽ
NODE* roots[2000];//���ڵ�������
NODE ANS[2000];//������
long long int right_counter = 0;//��¼��ȷ����

bool check(NODE* T_1, NODE* T_2) //���θ��ڵ�
{
    if (T_1 == nullptr && T_2 == nullptr)
    {//����������Ϊ�գ�һ��ͬ��
        return true;
    }
    else if (T_1 == nullptr || T_2 == nullptr)
    {//���һ�������գ���һ�����գ�һ����ͬ��
        return false;
    }
    else
    {//������������
        if ((T_1->op != T_2->op) || (T_1->kind != T_2->kind) || (T_1->type != T_2->type) ||
            (T_1->fenzi != T_2->fenzi) || (T_1->fenmu != T_2->fenmu)
            )
        {//����������ĵ�ǰ�ڵ����ݲ�һ�£�һ����ͬ��(���ڵ�op��ͬ)
            return false;
        }
        else {//�ڵ�һ��
            if (T_1->leftchild == nullptr && T_2->leftchild == nullptr) {//�����������Ϊ�գ��Ƚ�������
                return check(T_1->rightchild, T_2->rightchild);
            }
            else if (T_1->leftchild == nullptr || T_2->leftchild == nullptr) {//���һ��������Ϊ����һ����Ϊ�գ��ز�ͬ��
                return false;
            }
            else {
                if ((T_1->leftchild->type == T_2->leftchild->type) && (T_1->leftchild->fenzi == T_2->leftchild->fenzi) &&
                    (T_1->leftchild->fenmu == T_2->leftchild->fenmu) &&
                    (T_1->leftchild->op == T_2->leftchild->op) && (T_1->leftchild->kind == T_2->leftchild->kind))
                    //(T_1->leftchild->data == T_2->leftchild->data) 
                {//�������������Ϊ�գ���ֵ��ͬ�������ж���������
                    return check(T_1->leftchild, T_2->leftchild) && check(T_1->rightchild, T_2->rightchild);
                }
                else
                {//�����������ֵ��ͬ���Ҵ�ʱΪ �� + �� ��������� �� * �� ����������������������Ƿ�ͬ��
                    if (T_1->op == 0 || T_2->op == 2) {
                        return check(T_1->leftchild, T_2->rightchild) && check(T_1->rightchild, T_2->leftchild);
                    }
                    else {//���򣬲�ͬ��
                        return false;
                    }
                }
            }
        }
    }
    //cout << "hello";
}

bool checksame(int k)//��鵱ǰ���ڵ������֮ǰ�Ľڵ�֮���Ƿ�ͬ��
{
    for (int i = 0; i < k; i++)
    {
        if (check(roots[i], roots[k]))//����ͬ��
        {
            cout << "��roots[" << i << "]ͬ��" << endl;
            return true;
        }
    }
    return false;
}

void expression(NODE* nd)//���ɺ�׺���ʽ
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

void print(node* nd)//��ӡ���ʽ
{
    int sign = 0;//ÿ�εݹ����Ƿ���Ҫ������
    if (nd->leftchild == NULL && nd->rightchild == NULL)
    {

        if (nd->type == 0)
            cout << nd->fenzi;
        else if (nd->type == 1)
            cout << nd->fenzi << "|" << nd->fenmu;

    }
    else
    {
        if (nd->leftchild != NULL)
        {
            if (((nd->op == 0 || nd->op == 1) && (nd->leftchild->op == 2 || nd->leftchild->op == 3 || nd->leftchild->op == 10000)) || ((nd->op == 2 || nd->op == 3) && (nd->leftchild->op == 10000)))
                //�ӷ����������ȼ����ڳ˷�����������op���ȼ���ߣ�Ϊ��ӡ����׼����
                print(nd->leftchild);
            else
            {
                sign++;
                cout << '(';
                print(nd->leftchild);
            }
        }
        //һ�εݹ����ֻ���ܼ�һ������
        if (sign > 0)//���һ�������ı��������ж������ŵ�����
        {
            cout << ')';
            sign--;
        }
        cout << operate[nd->op];
        if (nd->rightchild != NULL)
        {
            if (((nd->op == 0 || nd->op == 1) && (nd->rightchild->op == 2 || nd->rightchild->op == 3 || nd->rightchild->op == 10000)) || ((nd->op == 2 || nd->op == 3) && (nd->rightchild->op == 10000)))
                print(nd->rightchild);
            else
            {
                sign++;
                cout << '(';
                print(nd->rightchild);
            }
        }
        if (sign > 0)//���һ�������ı��������ж������ŵ�����
        {
            cout << ')';
            sign--;
        }

    }

}

void out(NODE temp)//�����
{
    if (temp.flag == 1)
    {
        cout << "error!!!" << endl;
    }
    else
    {
        cout << "��ȷ���ǣ�      ";

        if (temp.kind == 0)
        {
            cout << temp.fenzi << endl;
        }
        else
        {
            if (temp.fenzi == 0)
            {
                cout << "0" << endl;
                //system("PAUSE");bug fixed date:2022 1 13
            }
            else if (temp.fenmu == 1)
            {
                cout << temp.fenzi <<"fixed" << endl;
                //system("PAUSE");
            }
            else
            {
                cout << temp.fenzi << "|" << temp.fenmu << endl;
            }
        }
    }

    for (int i = 0; i < 60; i++) {
        cout << "-";
    }
    cout << endl;
}

NODE calculate(NODE* nd)//����Ϊroots[i]
{
    for (unsigned int i = 0; i < save.size(); i++)//2022.01.14warning vector.sizeΪunsigned����
    {
        if (save[i]->type != 2)//����op
        {
            cal.push(*save[i]);
        }
        else
        {
            if (save[i]->kind == 0)//������ʽ
            {
                NODE a, b;
                a = cal.top();
                cal.pop();
                b = cal.top();
                cal.pop();
                NODE c;
                c.kind = 0;
                c.type = 0;
                c.op = 10000;
                c.fenzi = 0;
                c.fenmu = 0;
                if (save[i]->op == 0)//+
                    c.fenzi = a.fenzi + b.fenzi;
                else if (save[i]->op == 1)//-
                    c.fenzi = b.fenzi - a.fenzi;
                else if (save[i]->op == 2)//*
                    c.fenzi = b.fenzi * a.fenzi;
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
                        c.fenzi = b.fenzi / a.fenzi;
                    }
                }
                cal.push(c);
            }
            else//������ʽ
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
NODE* gen(int depth, int type)
//op=1���ַ���type=0��������type=1�Ƿ���
{
    NODE* tmp = (NODE*)malloc(sizeof(NODE));

    if (depth <= 0)//Ҷ�ڵ� num
    {
        if (tmp)//2022 1.14 ����warning �Կ�ָ�������
        {
            tmp->sign = 0;
            tmp->kind = type;
            tmp->leftchild = NULL;
            tmp->rightchild = NULL;
            if (type == 0)//����
            {
                tmp->type = 0;
                tmp->op = 10000;//��ӡʹ��
                tmp->fenzi = rand() % 99 + 1;
                tmp->fenmu = INT_MAX;
            }
            else if (type == 1)//����
            {

                tmp->type = 1;
                tmp->fenmu = rand() % 18 + 2;
                tmp->op = 10000;//��ӡʹ��
               // cout << "sign" << tmp->fenmu << "sign" << endl;
                tmp->fenzi = rand() % tmp->fenmu;
                //cout << "sign1" << tmp->fenzi << "sign1" << endl;
            }
        }
    }
    else//���ڵ㣬op
    {
        if (tmp)
        {
            tmp->sign = 1;
            tmp->kind = type;
            tmp->type = 2;//����������������ţ�
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

//�ж��û������Ƿ���ȷ
void check_userAnswer(NODE n) {
    
    //�û�����
    string user_answer;
    cin >> user_answer;

    //������ȷ��
    string ans_right;
    if (n.type == 0) {//��ȷ��Ϊ����
        ans_right = to_string(n.fenzi);
    }
    else {//��ȷ��Ϊ����
        string ans_right_fenzi;
        string ans_right_fenmu;

        //���ӷ�ĸ�ֱ���Ϊ�ַ���
        ans_right_fenzi = to_string(n.fenzi);
        ans_right_fenmu = to_string(n.fenmu);

        //ƴ��Ϊһ�����ַ���
        ans_right = ans_right_fenzi + "|" + ans_right_fenmu;
    }

    //�Ƚϲ���
    if (user_answer != ans_right) {
        cout << "��Ĵ𰸴���" << endl;
    }
    else {
        right_counter++;
        cout << "��ϲ�������" << endl;
    }
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

        if (checksame(i) == true)//��������ظ��ˣ��������
        {
            cout << "��ǰ���ʽ��";
            print(roots[i]);
            cout << "����ͬ��" << endl;
            system("PAUSE");
            i--;
            continue;
        }
        else
        {
            expression(roots[i]);
            ANS[i] = calculate(roots[i]);//�洢��
            if (ANS[i].flag == 1)//��0����
            {
                i--;
                while (!save.empty())//���save
                {
                    save.pop_back();
                }
                continue;
            }
            else
            {
                cout << "NO " << i + 1 << ":" << endl;
                print(roots[i]);
                cout << endl;

                check_userAnswer(ANS[i]);
                
                out(ANS[i]);//�����

                while (!save.empty())//���save
                {
                    save.pop_back();
                }
            }
        }
    }
}


/*void print(node* nd)//������version
{

    if (nd->leftchild == NULL && nd->rightchild == NULL)
    {
        if (nd->type == 0)
            cout << nd->fenzi;
        else if (nd->type == 1)
            cout << nd->fenzi << "|" << nd->fenmu;
    }
    else
    {
        if (nd->leftchild != NULL)
        {
            print(nd->leftchild);
        }
        cout << operate[nd->op];
        if (nd->rightchild != NULL)
        {
            print(nd->rightchild);
        }
    }

}*/


int main()
{
    gentree(10);
    cout << "�����ȷ��Ϊ��      ";
    cout << (double)right_counter / (double)10  << endl;
  /*  NODE n;
    n.type = 1;
    n.fenzi = -16543;
    n.fenmu = 4;
    n.kind = 0;
    check_user(n);*/



    /*srand((int)time(0));
    for (int i = 0; i < 1000; i++)
    {
        expression(roots[i]);

        for (int i = 0; i<save.size(); i++)
        {
            if (save[i]->type == 0)
                cout << save[i]->fenzi<<".";
            else if (save[i]->type == 1)
                cout << save[i]->fenzi << "|" << save[i]->fenmu<<".";
            else
                cout << operate[save[i]->op]<<".";
        }

        cout << endl;


        ANS[i]=calculate(roots[i]);
        print(roots[i]);


        while (!save.empty())//���save
        {
            save.pop_back();
        }
        cout << endl;
        //out(ANS[i]);

    }*/


}
