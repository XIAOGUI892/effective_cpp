#include<string>
#include<iostream>
class Person {
public:
	Person() = default;
	~Person() = default;
private:
	std::string name;
	std::string address;
};
class Student:public Person {
public:
	Student() = default;
	~Student() = default;
private:
	std::string schoolname;
	std::string schooladdress;
};
bool validStudent1(Student s) { return false; };
bool validStudent2(const Student& s) { return true; };
int main() {
	Student stu;
	validStudent1(stu);
	//如果用pass-by-value开销在哪里
	//调用person对象的两个string对象的构造函数，调用person的构造函数，调用Student对象的两个string对象的构造函数，调用Student的构造函数
	//函数结束后栈空间销毁，调用Student的析构函数，调用Student对象的两个string对象的构造函数，调用person的构造函数，调用person对象的两个string对象的构造函数
	//开销非常大！！！
	//=============================================================================================
	validStudent2(stu);
	//pass-by-reference-to-const避免了调用函数的开销
	//还有一个好处，就是引用传递为多态提供了条件，如果用值传递，会造成切片的问题

	//当然也有例外，如：cpp内置类型的传参引用传递可能不是最优解，内置类型内存较小，而引用传递的底层是指针，使用引用传递通常意味着传递的是指针，所以值传递的
	//效果更好
	
	//但不是所有小型type都适合值传递，对象小但是构造函数析构函数的使用要有大量开销

	//通常pass-by-value适用于stl迭代器与函数对象，和内置类型
}