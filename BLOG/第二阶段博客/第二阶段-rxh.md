
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
    int mul = -1;// mul=0 ^ mul =1 **
    int zhishu = 1;//2-4

    struct node* leftchild = NULL;

    struct node* rightchild = NULL;

}NODE, * Node;
```

在原有基础上，添加乘方的标志mul，当mul是-1时，表示该数字无乘方运算，当mul为0时表示^模式，当mul为1时表示**模式，zhishu表示乘方的指数，初始化为1
</br>

2.生成运算表达式</br>

在之前基础上，添加了含有乘方运算表达式的生成,主要```gen()```函数上发生改动，```gentree()```函数与gen函数的调用关系不变。
```gen()```改动后代码如下

``` c++
NODE* gen(int depth, int type)
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
```

```gentree()```未发生变更。

3.生成后缀表达式
此部分未发生变化。

4.计算表达式
在原有基础上，添加了乘方运算模式，基本框架及算法原理未改变，增加了指数的细节处理。

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

添加乘方模式的代码如下</br>

``` c++
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
```

5.打印表达式
在之前的基础上，添加了乘法模式的选择打印
代码如下:

``` c++
void print(node* nd)//打印表达式
{
    int sign = 0;//每次递归标记是否需要加括号
    if (nd->leftchild == NULL && nd->rightchild == NULL)
    {
        if (nd->mul == -1)
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
            if (nd->type == 0)
            {
                cout << nd->fenzi;
                if (mode == 0)
                {
                    cout << "^";
                    temp = temp + to_string(nd->fenzi)+"^";//存文件准备
                }
                else if (mode == 1)
                {
                    cout << "**";
                    temp = temp + to_string(nd->fenzi)+"**";//存文件准备
                }
                cout << nd->zhishu;
                temp = temp + to_string(nd->zhishu);//存文件准备
            }
            else if (nd->type == 1)
            {
                cout << "(" << nd->fenzi << "|" << nd->fenmu << ")";
                if (mode == 0)
                {
                    cout << "^";
                    temp = temp + "(" + to_string(nd->fenzi) + "|" + to_string(nd->fenmu) + ")^";
                }
                else if (mode == 1)
                {
                    cout << "**";
                    temp = temp + "(" + to_string(nd->fenzi) + "|" + to_string(nd->fenmu) + ")**";
                }
                cout << nd->zhishu;
                temp = temp + to_string(nd->zhishu);
            }

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
此部分未发生变化

7.用户输入及正确性判断部分
该部分由队友完成，API为```void check_userAnswer(NODE n)```</br>

# 界面设计

此阶段未提出界面交互设计需求

# 结对过程描述

![图片1](http://114.115.245.158:8000/1120191761/math-generator/-/blob/main/BLOG/%E7%AC%AC%E4%BA%8C%E9%98%B6%E6%AE%B5%E5%8D%9A%E5%AE%A2/%E7%AC%AC%E4%BA%8C%E9%98%B6%E6%AE%B5%E8%AE%A8%E8%AE%BA%E8%BF%87%E7%A8%8B/Screenshot_20220207_114819_com.tencent.mobileqq.jpg)
![图片2](http://114.115.245.158:8000/1120191761/math-generator/-/blob/main/BLOG/%E7%AC%AC%E4%BA%8C%E9%98%B6%E6%AE%B5%E5%8D%9A%E5%AE%A2/%E7%AC%AC%E4%BA%8C%E9%98%B6%E6%AE%B5%E8%AE%A8%E8%AE%BA%E8%BF%87%E7%A8%8B/Screenshot_20220207_114825_com.tencent.mobileqq.jpg)

# 相互点评

优点</br>

1. 踏实肯干
2. 代码逻辑思路清晰，注释明确
3. 善于交流，使得双方合作顺畅
缺点</br>
暂未发现

# 阶段总结

第二阶段添加了乘方模式，并且为用户提供了不同的模式选择，总体的算法框架并未发生改变，只是添加了乘方的运算，这一阶段较为顺畅。

# psp表格

| psp 2.1                | 个人软件流程阶段                       | 预估耗时（分钟） | 实际耗时（分钟） |
| ---------------------- | -------------------------------------- | ---------------- | ---------------- |
| 规划                   | 计划                                   | 5                | 20               |
| 估计                   | 估计这个任务需要多少时间               | 10               | 5                |
| 发展                   | 开发                                   | 120             | 180             |
| 分析                   | 需求分析 (包括学习新技术)              | 30               | 35               |
| 设计规范               | 生成设计文档                           | 10               | 10               |
| 设计评审               | 设计复审（和同事设计文档）             | 10               | 10               |
| 编码标准               | 代码规范（为目前的开发制定合适的规范） | 10               | 10               |
| 设计                   | 具体设计                               | 5               | 10               |
| 编码                   | 具体编码                               | 180              | 120              |
| 代码审查               | 代码复审                               | 30               | 20               |
| 测试                   | 测试（自我测试，修改代码，提交修改）   | 20               | 30               |
| 报告                   | 报告                                   | 30               | 40               |
| 测试报告               | 测试报告                               | 10               | 20               |
| 尺寸测量               | 计算工作量                             | 10               | 10               |
| 事后分析和流程改进计划 | 事后总结，并提出过程改进计划           | 20               | 20               |
|                        | 合计                                   |     520         | 570             |
