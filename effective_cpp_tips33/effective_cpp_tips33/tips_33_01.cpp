#include<iostream>
class Base {
public:
	void f2() {};
	void f1(int) { std::cout << "Base f1(int)" << std::endl; }
	void f1() { std::cout << "Base f1" << std::endl; }
};
class Derived :public Base {
public:
	void f1(int num1, int num2) { std::cout << "Derived f1" << std::endl; }
};

int main() {
	Derived d;
	d.f1(1, 2);
	//报错d.f1();
	//报错d.f1(1);
	//原因：Derived重定义的f1函数覆盖了从基类来的f1函数，即使参数签名不同、即使基类的函数是虚函数
}