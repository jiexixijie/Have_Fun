// calc_stack.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stack>
#include <vector>
#include <sstream>
using namespace std;


vector<string> To_Formula(char const *p); //合并整数返回,变成中缀表达式
vector<string> Get_Rear(vector<string> &a);   //将中缀表达式转换为后缀表达式
bool Is_Symbol(char p); 
bool Is_Number(char p);
int Get_Priority(char symbol);   //获取符号优先级
void push_back(vector<string> &a, char p);  //push char 
double Calc(vector<string> &a);   //计算后缀表达式
double Get_Value(double data1, double data2, const string symbol);  // data1 symbol data2

int main(){
	while (true){
		printf("请输入要计算的表达式\n");
		string calc_string = "14*8/16+(92*10+10)";
		cin >> calc_string;
		vector<string> Formula = To_Formula(calc_string.c_str());
		Formula = Get_Rear(Formula);
		double result = Calc(Formula);
		printf("result:%s=%.1lf\n", calc_string.c_str(), result);
	}
	
	return 0;
}

vector<string> To_Formula(char const *p) {
	vector<string> result;
	while (*p != '\0') {
		if (Is_Symbol(*p)) {
			push_back(result, *p);
			p++;
		}
		else if (Is_Number(*p)) {
			int data = 0;
			//字符串转变成数字
			while (Is_Number(*p)) {
				data = data * 10 + (*p - '0');
				p++;
			}
			result.push_back(to_string(data));
		}
		else {
			printf("请输入正确的表达式(error:01)\n");
			exit(0);
		}
	}
	return result;
}

vector<string> Get_Rear(vector<string> &a) {
	//把中缀表达式转为后缀表达式
	stack<char> symbol;
	vector<string> Rear_For;
	vector<string>::iterator iter = a.begin();
	while (iter != a.end()) {
		char data = (*iter)[0];
		if (Is_Number(data)) {  //数字则直接压入
			Rear_For.push_back(*iter);
		}
		else {   //如果为符号
			if (symbol.empty() || data == '(') {   //栈为空  || (
				symbol.push(data);
			}
			else if (data == ')') {     //一直pop直到'('  并且不把'(' push 到vector中 
				while (symbol.top() != '(') {
					push_back(Rear_For, symbol.top());
					symbol.pop();
					if (symbol.empty()) {
						printf("请输入正确的表达式(error:02)\n");    //缺少对应的'('
						exit(0);
					}
				}
				symbol.pop();
			}
			else {    //比较运算符优先级  + - * / ^
				while (Get_Priority(data) < Get_Priority(symbol.top())) {   //优先级比之前的低就需要不断pop直到优先级比他小
					push_back(Rear_For, symbol.top());
					symbol.pop();
					if (symbol.empty()) {     //到空也跳出
						break;
					}
				}
				symbol.push(data);  //压入符号
			}
		}
		iter++;
	}
	while (!symbol.empty()){    //把所有的symbol表中放入vector中
		push_back(Rear_For, symbol.top());
		symbol.pop();
	}
	return Rear_For;
}

bool Is_Symbol(char p) {
	if (p == '(' || p == ')' || p == '*' || p == '/' || p == '+' || p == '-' || p == '^') {
		return true;
	}
	return false;
}

bool Is_Number(char p) {
	if (p <= '9'&&p >= '0') {
		return true;
	}
	return false;
}

int Get_Priority(char symbol) {
	if (symbol == '+' || symbol == '-') {
		return 1;
	}
	else if (symbol == '*' || symbol == '/') {
		return 2;
	}
	else if (symbol == '^') {
		return 3;
	}
	else if (symbol == '(') {
		return -1;
	}
	else if (symbol == ')') {   //需要开始pop直到 '）' 
		return 10;
	}
	return 0;
}

void push_back(vector<string> &a,char p) {
	char temp[2];
	temp[0] = p; temp[1] = '\0';
	a.push_back(temp);;
}

double Calc(vector<string> &a) {
	stack<double> data;
	vector<string>::iterator iter = a.begin();
	while (iter!=a.end()){   //数字就push
		if (Is_Number((*iter)[0])) {
			double temp = stod((*iter).c_str());
			data.push(temp);
		}
		else {     //碰到字符就弹出两个
			if (data.size() < 2) {   //不足两个
				printf("请输入正确的表达式(error:03)\n");
				exit(0);
			}
			double data1, data2;
			data2 = data.top();
			data.pop();
			data1 = data.top();
			data.pop();
			double value=Get_Value(data1, data2, *iter);
			data.push(value);
		}
		iter++;
	}
	if (data.size() != 1) {
		printf("请输入真确的表达式(error:04)\n");
		exit(0);
	}
	return data.top();
}

double Get_Value(double data1, double data2, const string symbol) {
	double result;
	if (symbol == "+") {
		result = data1 + data2;
	}
	else if (symbol == "-") {
		result = data1 - data2;
	}
	else if (symbol == "*") {
		result = data1 * data2;
	}
	else if(symbol == "/"){
		result = data1 / data2;
	}
	else if (symbol == "^") {
		result = pow(data1, data2);
	}
	else {
		printf("Unknow Symbol\n");
		exit(0);
	}
	return result;
}