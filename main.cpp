//直接任务
//完成请在这里进行标记
//1.struct使用类代替         ————————已完成
//2.#define用静态变量表示    ————————已完成
//3.char s[50]用string代替
//4.修改文件结构，三个文件 一个.h、一个放函数、一个放主函数
//5.split函数修改
//6.pri函数修改

//如果想具体了解BP算法：
//https://www.cnblogs.com/biaoyu/archive/2015/06/20/4591304.html
//https://blog.csdn.net/iterate7/article/details/79610700
//https://blog.csdn.net/qq_36764614/article/details/88774794

#include <iostream>
#include "Class.h"
using namespace std;
static const int epoch_num = 200; //轮次
static const double lr = 0.1;     //步长
static const int P_NUM = 4;        //隐藏层神经元数
double b[2];                       //两层的截距偏置
input_Point a[4];                  //输入层
Point p[P_NUM];                    //隐含层
Point h[3];                        //输出层
weight w(4, P_NUM, 3), wb(4, P_NUM, 3);             //w是整个网络的权值，每次都在更新。wb是备份w，有的地方要用到更新前的权值，这时就使用wb
int train_num = 40;
#include "function.h"

int main()
{
    init();//初始化
    data_read();//划分训练集和测试集
    for (int i = 0; i < epoch_num; i++) //训练epoch_num轮   
        r();
    pri(); //输出
    return 0;
}
