#include<string>
class Base {
private:
	int value1;
	Base& operator=(const Base& base) {
		value1 = base.value1;
	}
protected:
	Base(int value = 0):value1(value){}
	Base(Base& base) :value1(base.value1) {}
};
class Derived :public Base {
private:
	int value2;
public:
	Derived(int value = 0):value2(value){}
	Derived &operator=(const Derived& derived){
		if (this != &derived) {
			Base::operator=(derived);
			value2 = derived.value2;
		}
		return *this;
	}
};
int main() {
	Derived obj1(1);
	Derived obj2(2);
	obj1 = obj2;//编译错误,因为Base类的operator=函数被声明为private,所以Derived类无法访问它,因此编译器会报错。

}