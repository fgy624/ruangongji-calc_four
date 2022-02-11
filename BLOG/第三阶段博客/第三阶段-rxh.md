
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
用户->出题系统: 用户选择乘方模式
出题系统->出题系统: 生成初始算式
出题系统->出题系统: 进行判重以及除以0的操作 发现重复，重新生成
出题系统->出题系统: 生成成功
出题系统->用户: 显示所出题目
用户->出题系统: 提交答案
出题系统->出题系统: 判断答案正确数目
出题系统->用户: 将正确数目显示给用户
出题系统->用户: 是否显示历史记录
用户->出题系统: yes
出题系统->用户: 显示历史记录
用户->出题系统: no
出题系统->用户: 结束答题

```

# 模块接口的设计与实现过程

1.将原有代码封装进QT平台

封装后原有功能方法存入cal_card类中，类的相关方法定义如下

``` c++
private slots:
    void on_submit_clicked();
    void timekeeper();
private:
    Ui::Calc_card *ui;

    int time_counter_display=20;
    QTimer *ftime;
    QTime time_counter;

    int num_right=0;
    int num_question=1000;

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
        long long int fenzi = 0;
        //type==0,整数的值=分子的值
        //type==1，分数的值=分子的值/分母的值
        long long int fenmu = 0;
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

    int nqq;
```

2.倒计时功能实现
该部分由队友实现,接口为

``` c++
void Calc_card::timekeeper()
```

3.历史记录功能实现

该部分通过建立历史记录文档，在每次用户开始答题时获取当前时间并写入文档，之后，在用户每输入一道题的答案后，将用户答案与标准答案写入文档，并记录是否正确。最后在用户提交后将文件内容显示在textbrowser控件中。

``` c++
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
```


# 设计规范与异常处理

经商定，我们使用了相同的数据结构。</br>
此阶段暂无异常处理

# 界面设计

详情参见第三阶段设计文档。

# 结对过程描述

![图片1](http://114.115.245.158:8000/1120191761/math-generator/-/blob/main/BLOG/%E7%AC%AC%E4%B8%89%E9%98%B6%E6%AE%B5%E5%8D%9A%E5%AE%A2/%E7%AC%AC%E4%B8%89%E9%98%B6%E6%AE%B5%E7%BB%93%E5%AF%B9%E8%BF%87%E7%A8%8B/Screenshot_20220208_204007_com.tencent.mobileqq.jpg)
![图片2](http://114.115.245.158:8000/1120191761/math-generator/-/blob/main/BLOG/%E7%AC%AC%E4%B8%89%E9%98%B6%E6%AE%B5%E5%8D%9A%E5%AE%A2/%E7%AC%AC%E4%B8%89%E9%98%B6%E6%AE%B5%E7%BB%93%E5%AF%B9%E8%BF%87%E7%A8%8B/Screenshot_20220208_204005_com.tencent.mobileqq.jpg
)
![图片3](http://114.115.245.158:8000/1120191761/math-generator/-/blob/main/BLOG/%E7%AC%AC%E4%B8%89%E9%98%B6%E6%AE%B5%E5%8D%9A%E5%AE%A2/%E7%AC%AC%E4%B8%89%E9%98%B6%E6%AE%B5%E7%BB%93%E5%AF%B9%E8%BF%87%E7%A8%8B/Screenshot_20220208_203340_com.tencent.mobileqq.jpg
b)

# 相互点评

优点</br>

1. 踏实肯干
2. 代码逻辑思路清晰，注释明确
3. 善于交流，使得双方合作顺畅
缺点</br>
暂未发现

# 阶段总结

第三阶段实现了QT平台的移植，添加了倒计时以及历史记录功能，在开发过程中，对于信号的使用花费了一些时间，但最后完整的实现了功能，总体比较顺利。

# psp表格

| psp 2.1                | 个人软件流程阶段                       | 预估耗时（分钟） | 实际耗时（分钟） |
| ---------------------- | -------------------------------------- | ---------------- | ---------------- |
| 规划                   | 计划                                   | 5                | 20               |
| 估计                   | 估计这个任务需要多少时间               | 5                | 5                |
| 发展                   | 开发                                   | 1290             | 1330             |
| 分析                   | 需求分析 (包括学习新技术)              | 30               | 35               |
| 设计规范               | 生成设计文档                           | 60               | 50               |
| 设计评审               | 设计复审（和同事设计文档）             | 10               | 20               |
| 编码标准               | 代码规范（为目前的开发制定合适的规范） | 10               | 10               |
| 设计                   | 具体设计                               | 60               | 60               |
| 编码                   | 具体编码                               | 1000             | 1060             |
| 代码审查               | 代码复审                               | 30               | 20               |
| 测试                   | 测试（自我测试，修改代码，提交修改）   | 40               | 60               |
| 报告                   | 报告                                   | 100              | 90               |
| 测试报告               | 测试报告                               | 30               | 30               |
| 尺寸测量               | 计算工作量                             | 10               | 10               |
| 事后分析和流程改进计划 | 事后总结，并提出过程改进计划           | 30               | 30               |
|                        | 合计1                                  | 1325             | 1420             |