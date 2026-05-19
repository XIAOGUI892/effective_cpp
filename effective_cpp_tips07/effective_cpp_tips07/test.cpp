#include<iostream>
using namespace std;
class A {
private:
	int num;
public:
	A(int num) :num(num){
		cout << "A" <<num<< endl;
	}
};
class B {
	A a{ 0 };
public:
	B() {		
		cout << "B" << endl;
	}
};
int main() {
	B b;
	return 0;
}