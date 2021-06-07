#pragma once
#include<iostream>
#include<cstring>
#include <cmath>
using namespace std;
double sigmoid(double x) //激活函数sigmoid
{
    return 1 / (1 + exp(-1 * x));
}
void copy_w() //备份至wb
{
    wb.copy(w);
}
void bp1() //对隐含层权值w.w1进行反向传播更新
{
    double E_total = 0;
    double E_1 = 0, E_2 = 0, E_3 = 0;
    for (int p_num = 0; p_num < P_NUM; p_num++) //一共四个隐含层神经元
    {
        double new_t = 0; //new_t作为新的误差，计算完成后再进行set_t()储存
        for (int outp = 0; outp < 3; outp++)
        {
            new_t += h[outp].get_t() * wb.get_w2(p_num, outp);
        }
        new_t *= (p[p_num].get_out_num() * (1 - p[p_num].get_out_num())); //隐藏层误差=输出层误差之和 乘上out_p(1-out_p)
        p[p_num].set_t(new_t);
    }
    for (int p_num = 0; p_num < P_NUM; p_num++) //一共四个隐含层神经元
    {
        for (int w_num = 0; w_num < 4; w_num++) //一个隐含层神经元与4个输入层神经元相连接
        {
            double temp = w.get_w1(w_num, p_num); //temp作为新的权重，计算完成后再储存
            temp -= lr * (p[p_num].get_t() * a[w_num].get_num());
            w.set_w1(w_num, p_num, temp);
        }
        b[0] = b[0] - lr * p[p_num].get_t();
    }
}
void bp2(int label) //对输出层权值w.w2进行反向传播更新
{
    double E_total = 0;
    double E_1 = 0, E_2 = 0, E_3 = 0;
    for (int outp = 0; outp < 3; outp++) //三个outp
    {
        double target[3] = { 0 };
        target[label] = 1;
        E_1 = h[outp].get_out_num() - target[outp];
        E_2 = h[outp].get_out_num() * (1 - h[outp].get_out_num());
        h[outp].set_t(E_1 * E_2); //输出层误差

        for (int i = 0; i < P_NUM; i++) //四个w
        {
            E_3 = p[i].get_out_num();
            w.set_w2(i, outp,
                w.get_w2(i, outp) - lr * (E_1 * E_2 * E_3)); //更新权值
            b[1] = b[1] - lr * (E_1 * E_2);
        }
    }
}
void rr() //对隐含层、输出层进行计算赋值
{
    for (int i = 0; i < P_NUM; i++) //隐藏层的值
    {
        double new_num = a[0].get_num() * w.get_w1(0, i) + a[1].get_num() * w.get_w1(1, i) +
            a[2].get_num() * w.get_w1(2, i) + a[3].get_num() * w.get_w1(3, i) + b[0];
        p[i].set_num(new_num);
        p[i].set_out_num(sigmoid(new_num));
    }
    for (int i = 0; i < 3; i++) //三个输出层神经元
    {
        double new_num = p[0].get_num() * w.get_w2(0, i) + p[1].get_num() * w.get_w2(1, i) +
            p[2].get_num() * w.get_w2(2, i) + p[3].get_num() * w.get_w2(3, i) + b[1];
        h[i].set_num(new_num);
        h[i].set_out_num(sigmoid(new_num)); //进行激励函数
    }
    /*  这个注释掉的代码已重构为上面那个for循环  留在这里帮助理解
    h[0].num = p[0].num * w.w2[0][0] + p[1].num * w.w2[1][0] + p[2].num * w.w2[2][0] + p[3].num * w.w2[3][0];
    h[1].num = p[0].num * w.w2[0][1] + p[1].num * w.w2[1][1] + p[2].num * w.w2[2][1] + p[3].num * w.w2[3][1];
    h[2].num = p[0].num * w.w2[0][2] + p[1].num * w.w2[1][2] + p[2].num * w.w2[2][2] + p[3].num * w.w2[3][2];
    h[0].out_num = sigmoid(h[0].num);//进行激励函数
    h[1].out_num = sigmoid(h[1].num);
    h[2].out_num = sigmoid(h[2].num);
    */
}
void split(char* s, double& a, double& b, double& c, double& d, int& label)//把文件中的数据切片  这个函数乱写的 可以改掉
{
    //puts(s);
    int j = 0;                  //读取s的指针
    for (int i = 0; i < 4; i++) //abcd轮流
    {
        double sum = 0;
        int e = 1; //整数位
        for (; s[j] != ','; j++)
        {
            if (s[j] == '\n')
                break;
            if (s[j] == '.')
            {
                e = -1; //小数位
                continue;
            }
            if (e == 1)
                sum = sum * 10 + s[j] - '0';
            else if (e == -1) //小数位  这样写只能算到小数点后一位 是一个乱写的写法
                sum += (1.0 * (s[j] - '0')) / 10.0;
        }
        j++;       //跳过','
        switch (i) //赋值
        {
        case 0:
            a = sum;
            break;
        case 1:
            b = sum;
            break;
        case 2:
            c = sum;
            break;
        case 3:
            d = sum;
            break;
        }
    }
    if (!strcmp(&s[j], "Iris-setosa\n"))
        label = 0;
    else if (!strcmp(&s[j], "Iris-versicolor\n"))
        label = 1;
    else
        label = 2;
    //cout << label << endl;
}
void r() //进行神经元训练 每运行一次就是训练一次
{
    FILE* fp;
    fopen_s(&fp, "C:/Users/RZ/source/repos/C++/iris_train.txt", "r");
    char s[50] = { 0 };
    while (fgets(s, 50, fp)) //遍历文件中的数据
    {
        if (s[0] == '\n')
            break;
        double pa, pb, pc, pd;
        int label = 0;
        split(s, pa, pb, pc, pd, label); //从一行中提取需要的数据

        a[0].set_num_yi(pa); //输入层的值
        a[1].set_num_yi(pb);
        a[2].set_num_yi(pc);
        a[3].set_num_yi(pd);

        rr();       //
        bp1();      //更新输入层到隐含层
        bp2(label); //更新隐含层到输出层
        copy_w();   //备份w

        for (int i = 0; i < 50; i++) //置零
            s[i] = 0;
    }
    fclose(fp);
}

void pri() //最后输出结果的函数  有很多功能可以调用之前的函数 改一下变成调用函数 让这个函数不要这么臃肿
{
    FILE* fp;
    fopen_s(&fp, "C:/Users/RZ/source/repos/C++/iris_test.txt", "r");
    char s[50] = { 0 };
    int ci = 1;
    int acc_num = 0;
    int pri_num = 0;
    while (fgets(s, 50, fp))
    {
        if (s[0] == '\n')
            break;
        double pa, pb, pc, pd;
        int label = 0;
        split(s, pa, pb, pc, pd, label); //从文件中提取一行的数据

        a[0].set_num_yi(pa);
        a[1].set_num_yi(pb);
        a[2].set_num_yi(pc);
        a[3].set_num_yi(pd);

        rr(); //进行神经元计算

        if (h[0].get_out_num() > h[1].get_out_num() && h[0].get_out_num() > h[2].get_out_num())
            pri_num = 1;
        else if (h[1].get_out_num() > h[0].get_out_num() && h[1].get_out_num() > h[2].get_out_num())
            pri_num = 2;
        else if (h[2].get_out_num() > h[0].get_out_num() && h[2].get_out_num() > h[1].get_out_num())
            pri_num = 3;
        else
            cout << ci << "    h[0].out_num:" << h[0].get_out_num() << "    h[1].out_num:" << h[1].get_out_num() << "    h[2].out_num:" << h[2].get_out_num() << endl;

        //cout << ci << "    " << pri_num << endl;
        if (0 < ci && ci <= 50 - train_num)
            if (pri_num == 1)
                acc_num++;
        if (50 - train_num < ci && ci <= 100 - 2 * train_num)
            if (pri_num == 2)
                acc_num++;
        if (100 - 2 * train_num < ci && ci <= 150 - 3 * train_num)
            if (pri_num == 3)
                acc_num++;
        ci++;
    }
    cout << "acc: " << acc_num / (150.0 - 3 * train_num) * 100 << '%' << endl;
    fclose(fp);
}
void data_read()
{
    FILE* fp, * f_train, * f_test;
    fopen_s(&fp, "C:/Users/RZ/source/repos/C++/iris.txt", "r");
    fopen_s(&f_train, "C:/Users/RZ/source/repos/C++/iris_train.txt", "w");
    fopen_s(&f_test, "C:/Users/RZ/source/repos/C++/iris_test.txt", "w");
    char s[50] = { 0 };
    int ci = 0, label;
    double pa, pb, pc, pd;
    while (fgets(s, 50, fp))
    {
        if (s[0] == '\n')
            break;
        if (ci < train_num)
            fputs(s, f_train);
        else
            fputs(s, f_test);
        ci++;
        if (ci == 50)
            ci = 0;
        split(s, pa, pb, pc, pd, label);
        a[0].set_m(pa);
        a[1].set_m(pb);
        a[2].set_m(pc);
        a[3].set_m(pd);
    }
    fclose(fp);
    fclose(f_train);
    fclose(f_test);
}
void init()
{
    for (int i = 0; i < 3; i++) //权值初始化
    {
        for (int j = 0; j < P_NUM; j++)
        {
            w.set_w1(i, j, (i + j) / 10.0);
            w.set_w1(i + 1, j, (i + j) / 10.0);
            w.set_w2(j, i, (i + j) / 10.0);
        }
    }
    b[0] = 0.35;
    b[1] = 0.6;                           //初始化截距偏置
}