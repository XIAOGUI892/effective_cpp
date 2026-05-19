#include<iostream>
using namespace std;
class Base {
public:
	Base() { init(); }
	virtual void init(){
		cout << "Base init" << endl;	
	}
	~Base() {
		destroy();
	}
	virtual void destroy() {
		cout << "Base destroy" << endl;
	}
};
class Derived : public Base {
public:
	Derived() : Base() {
		init();
	}
	virtual void init() {
		cout << "Derived init" << endl;
	}
	~Derived() {		destroy();
	}
	virtual void destroy() {
		cout << "Derived destroy" << endl;
	}
};
int main() {
	Derived d;
	/*Base init  基类先构造，此时Derived的动态类型为Base，所以调用Base的init函数
Derived init   基类构造完成后，Derived的动态类型为Derived，所以调用Derived的init函数
Derived destroy  派生类先析构，此时Derived的动态类型为Derived，所以调用Derived的destroy函数
Base destroy    派生类析构完成后，Derived的动态类型为Base，所以调用Base的destroy函数*/

//总结：在构造函数和析构函数中调用虚函数是危险的，因为在构造过程中对象的动态类型还没有完全建立，
// 可能会调用到基类的版本而不是派生类的版本，导致意外的行为。因此，建议避免在构造函数和析构函数中调用虚函数。
	//那如何确保每一次Base体系下的的对象被创建，就能有适当的init函数被调用呢，详见tips_09_02.cpp
}