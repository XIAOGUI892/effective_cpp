//像一个问题：
//如果基类申请了堆上的内存，并且自定义了构造函数来进行深拷贝，那么派生类是否需要自定义构造函数来进行深拷贝呢？
// ================================================================================================================================
//1.如果派生类没有申请动态资源，那么派生类不需要自定义构造函数来进行深拷贝，因为基类的构造函数已经处理了动态资源的分配和复制。
#include<string>
class Base {
private:
	char* data;
public:
	Base(const char* str) {
		data = new char[strlen(str) + 1];
		strcpy(data, str);
	}
	Base(const Base& other) {
		data = new char[strlen(other.data) + 1];
		strcpy(data, other.data);
	}
	Base& operator=(const Base& other) {
		if (this != &other) {
			delete[] data;
			data = new char[strlen(other.data) + 1];
			strcpy(data, other.data);
		}
		return *this;
	}
	~Base() {
		delete[] data;
	}
};
class Derived :public Base {
public:
	//派生类的构造函数和赋值运算符不需要自定义，因为基类已经处理了动态资源的分配和复制
};
// ================================================================================================================================
//2.如果派生类也申请了动态资源，那么派生类需要自定义构造函数来进行深拷贝，以确保派生类的动态资源也被正确复制。
class Derived2 :public Base {
private:
	char* extraData;
public:
	Derived2(const char* strcat, const char* extra) :Base(strcat) {
		extraData = new char[strlen(extra) + 1];
		strcpy(extraData, extra);
	}
	Derived2(const Derived2& other) :Base(other) {
				extraData = new char[strlen(other.extraData) + 1];
				strcpy(extraData, other.extraData);
	}
	Derived2& operator=(const Derived2& other) {
		if (this != &other) {
			delete[] extraData;
			extraData = new char[strlen(other.extraData) + 1];
			strcpy(extraData, other.extraData);
			Base::operator=(other); //调用基类的赋值运算符来复制基类部分
		}
		return *this;
	}


};

