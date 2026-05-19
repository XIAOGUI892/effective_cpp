#include<iostream>
class Base {
public:
	void f2() {};
	void f1(int) { std::cout << "Base f1(int)" << std::endl; }
	void f1() { std::cout << "Base f1" << std::endl; }
};
class Derived :public Base {
public:
	using Base::f1;//Ê¹ÓÃusing ÉùÃ÷
	void f1(int num1, int num2) { std::cout << "Derived f1" << std::endl; }
};

int main() {
	Derived d;
	d.f1(1, 2);
	d.f1();
	d.f1(1);
	
}