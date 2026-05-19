#include<iostream>
class Base {
public:
	void f2() {};
	void f1(int) { std::cout << "Base f1(int)" << std::endl; }
	void f1() { std::cout << "Base f1" << std::endl; }
};
class Derived :private Base {
public:
	using Base::f1;
	void f1(int num) { Base::f1(num); }
	//Ë½ÓÐ¼Ì³ÐÑ¡ÔñÐÔ±©Â©
};

int main() {
	Derived d;
	d.f1(1);
	d.f1();
}