#include<iostream>
#include<stdio.h>
#include<cstring>
#include<cmath>
using namespace std;
//test
#define epoch_num 1000
#define lr 0.1
typedef struct input_Point
{
	double num;
	int w[4];
}input_Point;
typedef struct Point
{
	double num;
	double out_num;
	double t;//隐含层误差
	int w1[4];
	int w2[4];
}Point;
typedef struct output_Point
{
	double num;
	double out_num;
	double t;//输出层误差
	int w[4];
}output_Point;
typedef struct the_w
{
	double w1[4][4];
	double w2[4][3];
}the_w;
input_Point a[4];
Point p[4];
output_Point h[3];
the_w w,wb;
double sigmoid(double x)
{
	return 1 / (1 + exp(-1 * x));
}
void copy_w()//备份至wb
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			wb.w1[i][j] = w.w1[i][j];
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 3; j++)
			wb.w2[i][j] = w.w2[i][j];
}
void bp1(int label)
{
	double E_total = 0;
	double E_1 = 0, E_2 = 0, E_3 = 0;
	for (int outp = 0; outp < 3; outp++)//三个outp
	{
		double target[3] = { 0 };
		if (label == 0)
			target[0] =1;
		else if (label == 1)
			target[1] = 1;
		else if (label == 2)
			target[2] = 1;
		
		E_1 = h[outp].out_num - target[outp];
		E_2 = h[outp].out_num * (1 - h[outp].out_num);
		h[outp].t = E_1 * E_2;//输出层误差

		for (int i = 0; i < 4; i++)//四个w
		{
			E_3 = p[i].out_num;
			w.w2[i][outp] = w.w2[i][outp] - lr * (E_1 * E_2 * E_3);//更新权值
		}
	}
}
void bp2()
{
	double E_total = 0;
	double E_1 = 0, E_2 = 0, E_3 = 0;
	for (int p_num = 0; p_num < 4; p_num++)//一共四个隐含层神经元
	{
		p[p_num].t = 0;
		for (int outp = 0; outp < 3; outp++)
		{
			p[p_num].t += h[outp].t * wb.w2[p_num][outp];
		}
		p[p_num].t *=(p[p_num].out_num*(1 - p[p_num].out_num));//隐藏层误差=输出层误差之和 乘上out_p(1-out_p)
	}
	for (int p_num = 0; p_num < 4; p_num++)//一共四个隐含层神经元
	{
		for (int w_num = 0; w_num < 4; w_num++)//一个隐含层神经元与4个输入层神经元相连接
		{
			w.w1[w_num][p_num] -= lr*(p[p_num].t * a[w_num].num);
		}
	}
}
void rr(int label)
{
	for (int i = 0; i < 4; i++)//隐藏层的值
	{
		p[i].num = a[0].num * w.w1[0][i] + a[1].num * w.w1[1][i] + a[2].num * w.w1[2][i] + a[3].num * w.w1[3][i];
		p[i].out_num = sigmoid(p[i].num);
	}
	h[0].num = p[0].num * w.w2[0][0] + p[1].num * w.w2[1][0] + p[2].num * w.w2[2][0] + p[3].num * w.w2[3][0];
	h[1].num = p[0].num * w.w2[0][1] + p[1].num * w.w2[1][1] + p[2].num * w.w2[2][1] + p[3].num * w.w2[3][1];
	h[2].num = p[0].num * w.w2[0][2] + p[1].num * w.w2[1][2] + p[2].num * w.w2[2][2] + p[3].num * w.w2[3][2];
	h[0].out_num = sigmoid(h[0].num);//进行激励函数
	h[1].out_num = sigmoid(h[1].num);
	h[2].out_num = sigmoid(h[2].num);
	bp1(label);//更新隐含层到输出层
	bp2();//更新输入层到隐含层
	copy_w();
}
void split(char* s, double& a, double& b, double& c, double& d, int& label);
void r()
{
	FILE* fp = fopen("C:/Users/RZ/source/repos/Project2/iris.txt", "r");
	char s[50] = { 0 };
	while (fgets(s, 50, fp))
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

		rr(label);


		for (int i = 0; i < 50; i++)//置零
			s[i] = 0;
	}
	fclose(fp);
}
void split(char* s, double& a, double& b, double& c, double& d, int& label)
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
			else if (e == -1)//小数位
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
void pri()
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

		for (int i = 0; i < 4; i++)//隐藏层的值
		{
			p[i].num = a[0].num * w.w1[0][i] + a[1].num * w.w1[1][i] + a[2].num * w.w1[2][i] + a[3].num * w.w1[3][i];
			p[i].out_num = sigmoid(p[i].num);
		}
		h[0].num = p[0].num * w.w2[0][0] + p[1].num * w.w2[1][0] + p[2].num * w.w2[2][0] + p[3].num * w.w2[3][0];
		h[1].num = p[0].num * w.w2[0][1] + p[1].num * w.w2[1][1] + p[2].num * w.w2[2][1] + p[3].num * w.w2[3][1];
		h[2].num = p[0].num * w.w2[0][2] + p[1].num * w.w2[1][2] + p[2].num * w.w2[2][2] + p[3].num * w.w2[3][2];
		h[0].out_num = sigmoid(h[0].num);//进行激励函数
		h[1].out_num = sigmoid(h[1].num);
		h[2].out_num = sigmoid(h[2].num);

		if (h[0].out_num > h[1].out_num && h[0].out_num > h[2].out_num)
			pri_num = 1;
		else if (h[1].out_num > h[0].out_num && h[1].out_num > h[2].out_num)
			pri_num = 2;
		else if (h[2].out_num > h[0].out_num && h[2].out_num > h[1].out_num)
			pri_num = 3;
		else
		cout << ci << "    h[0].out_num:" << h[0].out_num << "    h[1].out_num:" << h[1].out_num << "    h[2].out_num:" << h[2].out_num << endl;
		
		cout <<ci<<"    "<< pri_num << endl;
		if (0<ci&&ci <= 50)
			if (pri_num == 1)
				acc_num++;
		if (50<ci&&ci <= 100)
			if (pri_num == 2)
				acc_num++;
		if (100<ci&&ci<= 150)
			if (pri_num == 3)
				acc_num++;
		ci++;
	}
	cout << "acc: " << acc_num/150.0*100<<'%' << endl;
	fclose(fp);
}
int main()
{
	for (int i = 0; i < 3; i++)//权值初始化
	{
		for (int j = 0; j < 4; j++)
		{
			w.w1[i][j] = (i+j)/10.0;
			w.w1[i + 1][j] = (i + j) / 10.0;
			w.w2[j][i] = (i + j) / 10.0;
		}
	}
	for (int i = 0; i < epoch_num; i++)
		r();
	pri();
	return 0;
}
