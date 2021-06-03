//直接任务
//完成请在这里进行标记
//1.struct使用类代替
//2.#define用静态变量表示    ————————已完成
//3.char s[50]用string代替
//4.修改文件结构，三个文件 一个.h、一个放函数、一个放主函数
//5.split函数修改
//6.pri函数修改

//如果想具体了解BP算法：
//https://www.cnblogs.com/biaoyu/archive/2015/06/20/4591304.html
//https://blog.csdn.net/iterate7/article/details/79610700
//https://blog.csdn.net/qq_36764614/article/details/88774794







#include<iostream>
#include<stdio.h>
#include<cstring>
#include<cmath>
using namespace std;
static const int epoch_num = 1400;//轮次
static const double lr = 0.01;//步长
static const int P_NUM = 4;//隐藏层神经元数
double b[2];//两层的截距偏置

typedef struct input_Point//输入层
{
	double num;
}input_Point;
typedef struct Point//隐含层
{
	double num;
	double out_num;
	double t;//隐含层误差
}Point;
typedef struct output_Point//输出层
{
	double num;
	double out_num;
	double t;//输出层误差
}output_Point;
typedef struct the_w//权值
{
	double w1[4][P_NUM];
	double w2[P_NUM][3];
}the_w;
input_Point a[4];//输入层
Point p[P_NUM];//隐含层
output_Point h[3];//输出层
the_w w, wb;//w是整个网络的权值，每次都在更新。wb是备份w，有的地方要用到更新前的权值，这时就使用wb
double sigmoid(double x)//激活函数sigmoid
{
	return 1 / (1 + exp(-1 * x));
}
void copy_w()//备份至wb
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < P_NUM; j++)
			wb.w1[i][j] = w.w1[i][j];
	for (int i = 0; i < P_NUM; i++)
		for (int j = 0; j < 3; j++)
			wb.w2[i][j] = w.w2[i][j];
}
void bp1()//对隐含层权值w.w1进行反向传播更新
{
	double E_total = 0;
	double E_1 = 0, E_2 = 0, E_3 = 0;
	for (int p_num = 0; p_num < P_NUM; p_num++)//一共四个隐含层神经元
	{
		p[p_num].t = 0;
		for (int outp = 0; outp < 3; outp++)
		{
			p[p_num].t += h[outp].t * wb.w2[p_num][outp];
		}
		p[p_num].t *= (p[p_num].out_num * (1 - p[p_num].out_num));//隐藏层误差=输出层误差之和 乘上out_p(1-out_p)
	}
	for (int p_num = 0; p_num < P_NUM; p_num++)//一共四个隐含层神经元
	{
		for (int w_num = 0; w_num < 4; w_num++)//一个隐含层神经元与4个输入层神经元相连接
		{
			w.w1[w_num][p_num] -= lr * (p[p_num].t * a[w_num].num);
		}
		b[0] = b[0] - lr * p[p_num].t;
	}
}
void bp2(int label)//对输出层权值w.w2进行反向传播更新
{
	double E_total = 0;
	double E_1 = 0, E_2 = 0, E_3 = 0;
	for (int outp = 0; outp < 3; outp++)//三个outp
	{
		double target[3] = { 0 };
		if (label == 0)
			target[0] = 1;
		else if (label == 1)
			target[1] = 1;
		else if (label == 2)
			target[2] = 1;

		E_1 = h[outp].out_num - target[outp];
		E_2 = h[outp].out_num * (1 - h[outp].out_num);
		h[outp].t = E_1 * E_2;//输出层误差

		for (int i = 0; i < P_NUM; i++)//四个w
		{
			E_3 = p[i].out_num;
			w.w2[i][outp] = w.w2[i][outp] - lr * (E_1 * E_2 * E_3);//更新权值
			b[1] = b[1] - lr * (E_1 * E_2);
		}
	}
}
void rr()//对隐含层、输出层进行计算赋值
{
	for (int i = 0; i < P_NUM; i++)//隐藏层的值
	{
		p[i].num = a[0].num * w.w1[0][i] + a[1].num * w.w1[1][i] + a[2].num * w.w1[2][i] + a[3].num * w.w1[3][i] + b[0] + b[0];
		p[i].out_num = sigmoid(p[i].num);
	}
	for (int i = 0; i < 3; i++)//三个输出层神经元
	{
		h[i].num = p[0].num * w.w2[0][i] + p[1].num * w.w2[1][i] + p[2].num * w.w2[2][i] + p[3].num * w.w2[3][i] + b[1] + b[1];
		h[i].out_num = sigmoid(h[i].num);//进行激励函数
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
void split(char* s, double& a, double& b, double& c, double& d, int& label);
void r()//进行神经元训练 每运行一次就是训练一次
{
	FILE* fp = fopen("C:/Users/RZ/source/repos/Project2/iris.txt", "r");
	char s[50] = { 0 };
	while (fgets(s, 50, fp))//遍历文件中的数据
	{
		if (s[0] == '\n')
			break;
		double pa, pb, pc, pd;
		int label = 0;
		split(s, pa, pb, pc, pd, label);//从文件中提取一行的数据

		a[0].num = pa;//输入层的值
		a[1].num = pb;
		a[2].num = pc;
		a[3].num = pd;

		rr();//
		bp1();//更新输入层到隐含层
		bp2(label);//更新隐含层到输出层
		copy_w();//备份w

		for (int i = 0; i < 50; i++)//置零
			s[i] = 0;
	}
	fclose(fp);
}
void split(char* s, double& a, double& b, double& c, double& d, int& label)//把文件中的数据切片  这个函数乱写的 可以改掉
{
	//puts(s);
	int j = 0;//读取s的指针
	for (int i = 0; i < 4; i++)//abcd轮流
	{
		double sum = 0;
		int e = 1;//整数位
		for (; s[j] != ','; j++)
		{
			if (s[j] == '\n')break;
			if (s[j] == '.')
			{
				e = -1;//小数位
				continue;
			}
			if (e == 1)
				sum = sum * 10 + s[j] - '0';
			else if (e == -1)//小数位  这样写只能算到小数点后一位 是一个乱写的写法
				sum += (1.0 * (s[j] - '0')) / 10.0;
		}
		j++;//跳过','
		switch (i)//赋值
		{
		case 0:a = sum; break;
		case 1:b = sum; break;
		case 2:c = sum; break;
		case 3:d = sum; break;
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
void pri()//最后输出结果的函数  有很多功能可以调用之前的函数 改一下变成调用函数 让这个函数不要这么臃肿
{
	FILE* fp = fopen("C:/Users/RZ/source/repos/Project2/iris.txt", "r");
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
		split(s, pa, pb, pc, pd, label);//从文件中提取一行的数据

		a[0].num = pa;
		a[1].num = pb;
		a[2].num = pc;
		a[3].num = pd;

		rr();//进行神经元计算

		if (h[0].out_num > h[1].out_num && h[0].out_num > h[2].out_num)
			pri_num = 1;
		else if (h[1].out_num > h[0].out_num && h[1].out_num > h[2].out_num)
			pri_num = 2;
		else if (h[2].out_num > h[0].out_num && h[2].out_num > h[1].out_num)
			pri_num = 3;
		else
			cout << ci << "    h[0].out_num:" << h[0].out_num << "    h[1].out_num:" << h[1].out_num << "    h[2].out_num:" << h[2].out_num << endl;

		//cout << ci << "    " << pri_num << endl;
		if (0 < ci && ci <= 50)
			if (pri_num == 1)
				acc_num++;
		if (50 < ci && ci <= 100)
			if (pri_num == 2)
				acc_num++;
		if (100 < ci && ci <= 150)
			if (pri_num == 3)
				acc_num++;
		ci++;
	}
	cout << "acc: " << acc_num / 150.0 * 100 << '%' << endl;
	fclose(fp);
}
int main()
{
	for (int i = 0; i < 3; i++)//权值初始化
	{
		for (int j = 0; j < P_NUM; j++)
		{
			w.w1[i][j] = (i + j) / 10.0;
			w.w1[i + 1][j] = (i + j) / 10.0;
			w.w2[j][i] = (i + j) / 10.0;
		}
	}
	b[0] = 0.35; b[1] = 0.6;//初始化截距偏置
	for (int i = 0; i < epoch_num; i++)//训练几轮
		r();
	pri();//输出
	return 0;
}
