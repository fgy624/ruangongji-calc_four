# 1. UML图表述实体间关系

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

# 2. 模块接口的设计

1. 实现信息隐藏</br>
  通过对成员变量使用private属性，使得变量对用户不可见</br>
2. 界面设计</br>
   本阶段无界面设计</br>
3. 低耦合要求</br>
   不同类的成员函数之间不应该存在互相调用关系

# 3. 实现过程

## 3.1 数据结构的统一定义

经过沟通。我们绝定采用数这一数据结构来表示算式。核心的数据结构是$node$，代表树的节点，其中type可以区分数据算式的类型或者运算符，其他成员如下所示；

```c++
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

在后面的检验，计算等函数中，直接调用二叉树的根节点就可以了。



##  3.2 判断算式是否和之前出过的重复

由于采用了二叉树作为算式的表达方式，在判断重复这里我们考虑到可以利用二叉树的同构判断来解决。这一模块主要分为两个部分。首先是判断两个算式是否是重复的、这一部分的实现原理是判断两个二叉树是否同构，同构即代表算式是重复的，不同构即代表算式是不重复的，这里要注意的是只有 “+” 和 “*” 才可以交换左右子树，具体实现如下：

```c++
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
```

另一部分是和之前出过的算式比较，是否有重复的，这里就是将之前出过算式存储在一个数组中，需要比较时从头遍历即可，具体实现如下：

```c++
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
```



## 3.3 计算表达式

这里通过后序遍历树来生成后缀表达式，之后再利用运算栈$cal$来进行运算。我主要负责的是整数的部分，分数部分由队友实现。具体过程为：遍历后缀表达式，如遇见了运算数，就将其压入栈中，遇到操作符，就弹出栈顶两个元素，进行运算后将结果压入栈中。流程图如下：

```flow
start=>start: 开始
init=>operation: i=0
judge_length=>condition: i>=length?
judge_kind=>condition: s[i]为数字？
operation_num=>operation: 入栈
operation_op=>operation: 记录运算符为&
opp1=>operation: 弹出栈顶元素，记为a
opp2=>operation: 弹出栈顶元素，记为b
opp3=>operation: 记c=b&a，并将c压入栈中
opp_f=>operation: i++
opp_ff=>operation: 返回栈顶元素

start(right)->init
init->judge_length
judge_length(no,bottom)->judge_kind
judge_kind(no,bottom)->operation_op
operation_op->opp1
opp1->opp2
opp2->opp3
opp3->opp_f
judge_kind(yes,right)->operation_num
operation_num->opp_f
opp_f(left)->judge_length
judge_length(yes,right)->opp_ff
```





具体实现如下：

```c++
NODE calculate(NODE* nd)//传参为roots[i]
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



## 3.4 打印表达式

这里由于我主要负责的是判重和部分的计算，所以这里的打印我只打印了无括号的表达式，这里采用了前序遍历树来得到表达式，具体实现如下：

```c++
void print(node* nd)//无括号version
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

}
```

实际上这里没有打印括号的部分，这一部分的具体实现由队友实现



## 3.5 用户输入，判断正误及统计正确率部分

这里的设计思路是，首先由程序生成一道题，用户输入答案，形式为字符串。将正确的结果转化为字符串后与用户的答案进行比对，若相等则说明用户输入的答案正确，否则输入的答案错误。

统计正确率部分的设计思路为：首先有一个全局变量来记录用户答题的正确数，待全部题答完后将其与题的总数相除得到正确率，具体实现如下：

```c++
//判断用户输入是否正确
void check_userAnswer(NODE n) {
    
    //用户输入
    string user_answer;
    cin >> user_answer;

    //处理正确答案
    string ans_right;
    if (n.type == 0) {//正确答案为整数
        ans_right = to_string(n.fenzi);
    }
    else {//正确答案为分数
        string ans_right_fenzi;
        string ans_right_fenmu;

        //分子分母分别处理为字符串
        ans_right_fenzi = to_string(n.fenzi);
        ans_right_fenmu = to_string(n.fenmu);

        //拼接为一整个字符串
        ans_right = ans_right_fenzi + "|" + ans_right_fenmu;
    }

    //比较部分
    if (user_answer != ans_right) {
        cout << "你的答案错了" << endl;
    }
    else {
        right_counter++;
        cout << "恭喜答对啦！" << endl;
    }
}
```



# 4. 设计规范与异常处理

经过沟通，我们采用了共同的数据结构，此处详见2.1节，在程序的相应位置写下注释，

此阶段暂无异常。



# 5. 界面设计

由于第一阶段是命令行程序，故未涉及界面设计部分



# 6. 结对过程描述

如图所示：

![图片1](http://114.115.245.158:8000/1120191761/math-generator/-/blob/main/BLOG/%E7%AC%AC%E4%B8%80%E9%98%B6%E6%AE%B5%E5%8D%9A%E5%AE%A2/%E7%AC%AC%E4%B8%80%E9%98%B6%E6%AE%B5%E8%AE%A8%E8%AE%BA%E8%BF%87%E7%A8%8B/1.jpg)
![图片2](http://114.115.245.158:8000/1120191761/math-generator/-/blob/main/BLOG/%E7%AC%AC%E4%B8%80%E9%98%B6%E6%AE%B5%E5%8D%9A%E5%AE%A2/%E7%AC%AC%E4%B8%80%E9%98%B6%E6%AE%B5%E8%AE%A8%E8%AE%BA%E8%BF%87%E7%A8%8B/2.png)
![图片3](http://114.115.245.158:8000/1120191761/math-generator/-/commit/5e4e8f011d405c552a56e6586fdd3ecea0cda42b)



# 7. 点评队友

**总体上**：

​	在整个第一阶段的过程中，两人合作并未出现较大问题。讨论过程较为顺畅，分工明确，合作较为愉快。

**优点：**

1. 踏实肯干
2. 思路清晰
3. 编码能力强

**缺点：**

​	暂未发现缺点，合作融洽



# 8. 阶段总结

在第一阶段的设计和实现过程中，主要在于对于二叉树的相关内容的回顾。这一部分我主要是通过csdn来得到问题的答案。这一阶段增强了我解决问题的能力，同时也锻炼我与人合作共同完成项目的能力。



# 9. PSP表格
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
| 编码                   | 具体编码                               | 1000             | 1060             |
| 代码审查               | 代码复审                               | 30               | 20               |
| 测试                   | 测试（自我测试，修改代码，提交修改）   | 40               | 60               |
| 报告                   | 报告                                   | 80               | 70               |
| 测试报告               | 测试报告                               | 30               | 30               |
| 尺寸测量               | 计算工作量                             | 10               | 10               |
| 事后分析和流程改进计划 | 事后总结，并提出过程改进计划           | 30               | 30               |
|                        | 合计                                   | 1285             | 1390             |

