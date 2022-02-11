# 1. 实现过程

## 1.1 数据结构的统一定义

经过沟通。我们绝定采用二叉树这一数据结构来表示算式。核心的数据结构是$node$，代表树的节点，其中type可以区分数据算式的类型或者运算符，其他成员如下所示；

第二阶段主要更改的地方，在原有的基础上加了两个成员：$mul$l和$zhishu$，其中$mul$代表乘方符号的类型：0代表^，1代表**，$zhishu$代表指数，初始化为1。

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
    
    //第二阶段
    int mul = -1;// mul=0 ^ mul =1 **
    int zhishu = 1;//2-4
	//第二阶段end
    
    struct node* leftchild = NULL;

    struct node* rightchild = NULL;

}NODE, * Node;
```

在后面的检验，计算等函数中，直接调用二叉树的根节点即可。



##  1.2 判断算式是否和之前出过的重复

由于采用了二叉树作为算式的表达方式，在判断重复这里我们考虑到可以利用二叉树的同构判断来解决。这一模块主要分为两个部分。首先是判断两个算式是否是重复的、这一部分的实现原理是判断两个二叉树是否同构，同构即代表算式是重复的，不同构即代表算式是不重复的，这里要注意的是只有 “+” 和 “*” 才可以交换左右子树，具体实现如下：

第二阶段重点修改的是判断左右两棵子树的根节点是否相同的部分。这里加上了$mul$和$zhishu$的判断条件。

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
                //第二阶段
                if ((T_1->leftchild->type == T_2->leftchild->type) && (T_1->leftchild->fenzi == T_2->leftchild->fenzi) &&
                    (T_1->leftchild->fenmu == T_2->leftchild->fenmu) &&
                    (T_1->leftchild->op == T_2->leftchild->op) && (T_1->leftchild->kind == T_2->leftchild->kind)
                    && (T_1->leftchild->zhishu == T_2->leftchild->zhishu)
                    )
                    //第二阶段end
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

第二阶段这里并未修改

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



## 1.3 计算表达式

这里通过后序遍历树来生成后缀表达式，之后再利用运算栈$cal$来进行运算。我主要负责的是整数的部分，分数部分由队友实现。具体过程为：遍历后缀表达式，如遇见了运算数，就将其压入栈中，遇到操作符，就弹出栈顶两个元素，进行运算后将结果压入栈中。流程图如下：

第二阶段主要修改实现了乘方运算。实现的过程是：在将数字投入计算前，先将数字与其携带的指数成员先进行乘方运算，再将结果投入到正常的运算过程中。

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



## 1.4 打印表达式

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



## 1.5 用户输入，判断正误及统计正确率部分



第二阶段修改了用户选择乘方显示模式的部分，添加上后的具体实现如下：

```c++
//第二阶段
cout << "请选择输入模式，输入0为'^'，输入1为'**'" << endl;
cin >> mode;
//第二阶段end

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



# 2. 设计规范与异常处理

此阶段暂无异常。



# 3. 界面设计

由于第一阶段是命令行程序，故未涉及界面设计部分



# 4. 结对过程描述

如图所示：

![图片1](http://114.115.245.158:8000/1120191761/math-generator/-/blob/main/BLOG/%E7%AC%AC%E4%BA%8C%E9%98%B6%E6%AE%B5%E5%8D%9A%E5%AE%A2/%E7%AC%AC%E4%BA%8C%E9%98%B6%E6%AE%B5%E8%AE%A8%E8%AE%BA%E8%BF%87%E7%A8%8B/Screenshot_20220207_114819_com.tencent.mobileqq.jpg)
![图片2](http://114.115.245.158:8000/1120191761/math-generator/-/blob/main/BLOG/%E7%AC%AC%E4%BA%8C%E9%98%B6%E6%AE%B5%E5%8D%9A%E5%AE%A2/%E7%AC%AC%E4%BA%8C%E9%98%B6%E6%AE%B5%E8%AE%A8%E8%AE%BA%E8%BF%87%E7%A8%8B/Screenshot_20220207_114825_com.tencent.mobileqq.jpg)

# 5. 点评队友

**总体上**：

​	在整个第一阶段的过程中，两人合作并未出现较大问题。讨论过程较为顺畅，分工明确，合作较为愉快。

**优点：**

1. 踏实肯干
2. 思路清晰
3. 编码能力强

**缺点：**

​	暂未发现缺点，合作融洽



# 6. 阶段总结

第二阶段添加了乘方运算，这一阶段的实现较为顺畅。



# 7. PSP表格

| psp 2.1                | 个人软件流程阶段                       | 预估耗时（分钟） | 实际耗时（分钟） |
| ---------------------- | -------------------------------------- | ---------------- | ---------------- |
| 规划                   | 计划                                   | 3                | 10               |
| 估计                   | 估计这个任务需要多少时间               | 5                | 5                |
| 发展                   | 开发                                   | 1210             | 1300             |
| 分析                   | 需求分析 (包括学习新技术)              | 30               | 35               |
| 设计规范               | 生成设计文档                           | 40               | 50               |
| 设计评审               | 设计复审（和同事设计文档）             | 10               | 20               |
| 编码标准               | 代码规范（为目前的开发制定合适的规范） | 10               | 10               |
| 设计                   | 具体设计                               | 10               | 10               |
| 编码                   | 具体编码                               | 200              | 250              |
| 代码审查               | 代码复审                               | 30               | 20               |
| 测试                   | 测试（自我测试，修改代码，提交修改）   | 40               | 60               |
| 报告                   | 报告                                   | 80               | 50               |
| 测试报告               | 测试报告                               | 30               | 30               |
| 尺寸测量               | 计算工作量                             | 10               | 10               |
| 事后分析和流程改进计划 | 事后总结，并提出过程改进计划           | 30               | 30               |
|                        | 合计                                   | 462              | 560              |

