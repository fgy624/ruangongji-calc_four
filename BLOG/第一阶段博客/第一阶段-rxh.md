
# 接口设计

1. 实现信息隐藏</br>
通过对成员变量使用private属性，使得变量对用户不可见</br>
2.界面设计</br>
本阶段无界面设计</br>
3.低耦合要求</br>
不同类的成员函数之间不应该存在互相调用关系

# UML

```sequence
title: 顺序图

participant 用户
participant 出题系统

用户->出题系统: 输入题目数目
出题系统->出题系统: 生成初始算式
出题系统->出题系统: 进行判重以及除以0的操作 发现重复，重新生成
出题系统->出题系统: 生成成功
出题系统->用户: 显示所出题目
用户->出题系统: 提交答案
出题系统->出题系统: 判断答案正确数目
出题系统->用户: 将正确数目显示给用户
```

# 模块接口的设计与实现过程

1.数据结构的统一定义
代码如下

``` c++

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
```

经沟通，当kind=0为整数算式。kind为1为分数算式；type=0时，该节点表示整数，type=1时，该节点表示分数，当type=2时，该节点为运算符；在本阶段定义op=0为+，op=1为-，op=2为*，op=3为/，并且后续可以通过op++实现运算符的拓展。flag=0为算式是否除0。
</br>

2.生成运算表达式</br>

利用二叉树的数据结构实现运算式子的生成。其中，叶子节点为数字（包括分数和整数），非叶节点为运算符。由于需求中要求最多使用10个运算符，这样就规定了树的深度小于等于4.因此可以随机生成深度
```int depth = rand() % 3 + 2;```，接着利用递归的思想，当
```depth<=0```时，必为叶子节点，可随机生成数据。当```depth>0```
，可以继续随机生成运算符，然后继续生成他的左右子树。这样规定就避免了随机生成的算式不符合运算符逻辑要求的可能性。（不包括同构的算式以及除以0的错误算式检测）
代码如下

``` c++
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
```

之后需要对运算表达式进行同构检测以及除以0检测，同构检测由队友完成.</br>
利用```gentree```调用```gen```函数，其中包括同构的检测以及除以0的检测</br>
除以0的检测在```calculate```函数中实现</br>
该段代码有待整合</br>
更新：引入写文件部分

``` c++
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

        if (checksame(i) == true)//如果发现重复了，这里改了
        {
            /*cout << "当前表达式：";
            print(roots[i]);
            cout << "产生同构" << endl;
            system("PAUSE");*/
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
                //存文件
                temp.clear();
                fstream f;
                f.open(".\\题库.txt", ios::app);
                print(roots[i]);
                cout << endl;
                f << "NO " << i + 1 << ":" << endl << temp << endl;
                temp.clear();

                check_userAnswer(ANS[i]);

                out(ANS[i]);//输出答案

                while (!save.empty())//清空save
                {
                    save.pop_back();
                }
            }
        }
    }
}
```

3.生成后缀表达式
通过后续遍历算式树生成后缀表达式,为后续计算表达式提供基础,其中，```vector save```负责存储每个运算表达式的后缀表达式根节点</br>
代码如下</br>

``` c++
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
```

4.计算表达式
计算表达式通过后续遍历树生成后缀表达式，其中整数部分由队友完成，分数部分由我完成。核心思想是遍历后缀表达式，当检测到数字时压入栈```cal```，当检测到运算符时，从栈中弹出两个操作数与该运算符进行运算，将结果压入栈中，最后当表达式遍历结束后，栈顶就是最终运算结果。存入```ans数组中```</br>
其中，在```calculate```函数中利用```flag```标识来确定是否为除以0的运算，在gen函数中使用。</br>
该函数的流程图</br>

```flow
st=>start: 运算开始
op0=>operation: int i=0
e=>end: 运算结束
op1=>operation: 数字入栈
op2=>operation: 从栈中弹出数字a，b
op3=>operation: 进行a与b的运算
op4=>operation: 计算结果入栈
op5=>operation: 取出栈顶元素作为最终结果
op6=>operation: i++

cond1=>condition: i<后缀表达式长度
cond2=>condition: 当前字符是否为数字

st->op0->cond1(yes)->cond2(yes)->op1->op6->cond1
cond2(no)->op2->op3->op4(left)->op6(left)->cond1

cond1(no)->op5
op5->e
```

合并后的代码如下</br>

``` c++
NODE calculate(NODE* nd)//传参为roots[i]
{
    for (unsigned int i = 0; i < save.size(); i++)//2022.01.17warning vector.size为unsigned类型
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
```

5.打印表达式
队友之前编写了无括号版本，在此加入括号进行优化，通过递归，在每层递归里只能最多存在一层括号，当递归当前层的优先级低于上一层优先级时，需要加入括号，同时引入sign标识，在该层递归结束之后通过判断sign是否大于0来决定是否需要加入右括号。</br>
更新：(引入存文件部分，在打印表达式的同时临时存储，并且写入文件中)
代码如下:

``` c++
void print(node* nd)//打印表达式
{
    int sign = 0;//每次递归标记是否需要加括号
    if (nd->leftchild == NULL && nd->rightchild == NULL)
    {

        if (nd->type == 0)
        {
            cout << nd->fenzi;
            temp = temp + to_string(nd->fenzi);//存文件准备
        }
        else if (nd->type == 1)
        {
            cout << nd->fenzi << "|" << nd->fenmu;
            temp = temp + to_string(nd->fenzi) + "|" + to_string(nd->fenmu);
        }
    }
    else
    {
        if (nd->leftchild != NULL)
        {
            if (((nd->op == 0 || nd->op == 1) && (nd->leftchild->op == 2 || nd->leftchild->op == 3 || nd->leftchild->op == 10000)) || ((nd->op == 2 || nd->op == 3) && (nd->leftchild->op == 10000)))
                //加法、减法优先级低于乘法除法且数字op优先级最高（为打印括号准备）
                print(nd->leftchild);
            else
            {
                sign++;
                cout << '(';
                temp = temp + "(";
                print(nd->leftchild);
            }
        }
        //一次递归最多只可能加一层括号
        if (sign > 0)//完成一个子树的遍历后需判断右括号的有无
        {
            cout << ')';
            temp = temp + ")";
            sign--;
        }
        cout << operate[nd->op];
        temp = temp + operate[nd->op];
        if (nd->rightchild != NULL)
        {
            if (((nd->op == 0 || nd->op == 1) && (nd->rightchild->op == 2 || nd->rightchild->op == 3 || nd->rightchild->op == 10000)) || ((nd->op == 2 || nd->op == 3) && (nd->rightchild->op == 10000)))
                print(nd->rightchild);
            else
            {
                sign++;
                cout << '(';
                temp = temp + "(";
                print(nd->rightchild);
            }
        }
        if (sign > 0)//完成一个子树的遍历后需判断右括号的有无
        {
            cout << ')';
            temp = temp + ")";
            sign--;
        }

    }

}
```

6.输出答案
通过判断NODE结构体重kind的值来决定输出格式</br>
代码如下：

``` c++
void out(NODE temp)//输出答案
{
    if (temp.flag == 1)
    {
        cout << "error!!!" << endl;
    }
    else
    {
        if (temp.kind == 0)
        {
            cout << temp.fenzi << endl;
        }
        else
        {
            if (temp.fenzi == 0)
            {
                cout << "0" << endl;
                //system("PAUSE");bug fixed date:2022 1 17
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
}
```

7. 用户输入及正确性判断部分
该部分由队友完成，API为```void check_userAnswer(NODE n)```</br>

# 设计规范与异常处理

经商定，我们使用了相同的数据结构，此处详情2.1节。</br>
此阶段暂无异常处理

# 界面设计

此阶段未提出界面交互设计需求

# 结对过程描述

![图片1](http://114.115.245.158:8000/1120191761/math-generator/-/blob/main/BLOG/%E7%AC%AC%E4%B8%80%E9%98%B6%E6%AE%B5%E5%8D%9A%E5%AE%A2/%E7%AC%AC%E4%B8%80%E9%98%B6%E6%AE%B5%E8%AE%A8%E8%AE%BA%E8%BF%87%E7%A8%8B/1.jpg)
![图片2](http://114.115.245.158:8000/1120191761/math-generator/-/blob/main/BLOG/%E7%AC%AC%E4%B8%80%E9%98%B6%E6%AE%B5%E5%8D%9A%E5%AE%A2/%E7%AC%AC%E4%B8%80%E9%98%B6%E6%AE%B5%E8%AE%A8%E8%AE%BA%E8%BF%87%E7%A8%8B/2.png)
![图片3](http://114.115.245.158:8000/1120191761/math-generator/-/commit/5e4e8f011d405c552a56e6586fdd3ecea0cda42b)

# 相互点评

优点</br>

1. 踏实肯干
2. 代码逻辑思路清晰，注释明确
3. 善于交流，使得双方合作顺畅
缺点</br>
暂未发现

# 阶段总结

技术难点：复习树等数据结构，以便于解决表达式求值。在编码过程中，两人合作非常顺畅，首先商讨确定数据结构，分工明确，为之后的对接奠定的基础。其次在编码过程中，提升了编写代码的能力以及沟通能力。

# psp表格

| psp 2.1                | 个人软件流程阶段                       | 预估耗时（分钟） | 实际耗时（分钟） |
| ---------------------- | -------------------------------------- | ---------------- | ---------------- |
| 规划                   | 计划                                   | 5                | 20               |
| 估计                   | 估计这个任务需要多少时间               | 5                | 5                |
| 发展                   | 开发                                   | 1210             | 1300             |
| 分析                   | 需求分析 (包括学习新技术)              | 30               | 35               |
| 设计规范               | 生成设计文档                           | 60               | 50               |
| 设计评审               | 设计复审（和同事设计文档）             | 10               | 20               |
| 编码标准               | 代码规范（为目前的开发制定合适的规范） | 10               | 10               |
| 设计                   | 具体设计                               | 10               | 10               |
| 编码                   | 具体编码                               | 800              | 960              |
| 代码审查               | 代码复审                               | 30               | 20               |
| 测试                   | 测试（自我测试，修改代码，提交修改）   | 50               | 90               |
| 报告                   | 报告                                   | 80               | 70               |
| 测试报告               | 测试报告                               | 30               | 30               |
| 尺寸测量               | 计算工作量                             | 10               | 10               |
| 事后分析和流程改进计划 | 事后总结，并提出过程改进计划           | 20               | 20               |
|                        | 合计                                   | 1085             | 1310             |