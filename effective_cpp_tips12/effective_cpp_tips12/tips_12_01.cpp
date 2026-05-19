#include<string>
using namespace std;
class Customer {
private:
	string name;
public:
	Customer(const string& name = "") :name(name) {}
	Customer(const Customer &other):name(other.name){}
	Customer& operator=(const Customer &other){
		name = other.name;
		return *this;
	}
};
class PriorityCustomer : public Customer {
private:
	int priority;
public:
	PriorityCustomer(const PriorityCustomer&other):priority(other.priority){}
	PriorityCustomer& operator=(const PriorityCustomer& other) {
		priority = other.priority;
		return *this;
	}
	//问题出现：用户自己定义了拷贝函数，但没有调用基类的拷贝函数，导致基类部分没有被正确复制
	//基类有的成员是私有的，派生类无法直接访问，所以必须调用基类的拷贝函数来复制基类部分

};
class PriorityCustomer2 : public Customer {
private:
	int priority;
public:
	PriorityCustomer2(const PriorityCustomer2&other):Customer(other),priority(other.priority){}
	PriorityCustomer2& operator=(const PriorityCustomer2& other) {
		Customer::operator=(other); //调用基类的赋值运算符
		priority = other.priority;
		return *this;
	}
};
