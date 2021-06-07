#pragma once
#include<iostream>
using namespace std;
class input_Point //输入层
{
private:
    double num;

public:
    double max;//max和min是进行归一化时所需要的
    double min;
    input_Point();
    double get_num() const { return num; }
    void set_num(double new_num) {num = new_num;}
    void set_num_yi(double new_num) { num = (new_num-min)/(max-min); }
    void set_m(double a);
};
input_Point::input_Point()
{
    num = 0;
    max = 0;
    min = 100;
}
void input_Point::set_m(double a)
{
    if (a > max)
        max = a;
    if (a < min)
        min = a;
}
class Point : public input_Point //隐含层和输出层
{
    //隐藏层相比输入层 多了一个需要通过激活得到的输出值 out_num
private:
    double out_num;
    double t; //隐含层误差
public:
    double get_out_num() const { return out_num; }
    void set_out_num(double new_out_num) { out_num = new_out_num; }
    double get_t() const { return t; }
    void set_t(double new_t) { t = new_t; }
};
class weight //权值
{
private:
    double** w1, ** w2; //w1为输入层->隐藏层权重，w2为隐藏层->输出层权重
    int a, b, c;       //分别为输入层，隐藏层，输出层节点数
public:
    weight(int a = 1, int b = 1, int c = 1); //构造函数，构造w1[a][b],w2[b][c]
    void copy(const weight& new_w);          //深拷贝，备份权重的时候要用
    ~weight();
    double get_w1(int i, int j) const { return w1[i][j]; }        //获取w1[i][j]
    void set_w1(int i, int j, double value) { w1[i][j] = value; } //设置w1[i][j]
    double get_w2(int i, int j) const { return w2[i][j]; }        //获取w2[i][j]
    void set_w2(int i, int j, double value) { w2[i][j] = value; } //设置w2[i][j]
    int get_a() const { return a; }
    int get_b() const { return b; }
    int get_c() const { return c; }
};
weight::weight(int new_a, int new_b, int new_c)
{
    this->a = new_a; //记录，之后析构释放内存的时候要用到
    this->b = new_b;
    this->c = new_c;
    //w1设置为[a][b]二维数组
    w1 = new double* [a];
    for (int i = 0; i < a; i++)
        w1[i] = new double[b];
    //w2设置为[b][c]二维数组
    w2 = new double* [b];
    for (int i = 0; i < b; i++)
        w2[i] = new double[c];
}
void weight::copy(const weight& new_w)
{
    //和构造函数一样的分配内存
    this->a = new_w.a;
    this->b = new_w.b;
    this->c = new_w.c;
    w1 = new double* [a];
    for (int i = 0; i < a; i++)
        w1[i] = new double[b];
    w2 = new double* [b];
    for (int i = 0; i < b; i++)
        w2[i] = new double[c];
    //复制数据
    for (int i = 0; i < a; i++) //复制w1
        for (int j = 0; j < b; j++)
            w1[i][j] = new_w.w1[i][j];
    for (int i = 0; i < b; i++) //复制w2
        for (int j = 0; j < c; j++)
            w2[i][j] = new_w.w2[i][j];
}
weight::~weight()
{
    for (int i = 0; i < a; i++)
        delete[] w1[i];
    for (int i = 0; i < b; i++)
        delete[] w2[i];
    delete[] w1;
    delete[] w2;
}
