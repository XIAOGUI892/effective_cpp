class Myclass1 {
private:
	Myclass1(const Myclass1&);
	Myclass1& operator=(const Myclass1&);
public:
	Myclass1() {};
};
//为了防止类被复制，声明了一个私有的复制构造函数和赋值运算符，并且没有提供它们的定义。
// 这样，如果有人试图复制一个Myclass对象，编译器会报错，因为复制构造函数和赋值运算符是不可访问的。这是一种常见的技巧，用于禁止类的复制行为。
//需要注意的是，如果类中有friend函数或成员函数企图复制对象，会在链接时报错，因为没有具体实现复制构造函数和赋值运算符，详见下面的示例：
class Myclass2 {
private:
	Myclass2(const Myclass2&);
	Myclass2& operator=(const Myclass2&);
public:
	Myclass2() {};
	friend Myclass2 copy_Myclass2(const Myclass2& obj) {
		Myclass2 obj1 = obj;
		return obj1;
	}
	Myclass2 member_fun() {
		Myclass2 obj1 = *this;
		return obj1;
	}
	Myclass2 member_fun2(const Myclass2& obj) {//不能用值传递参数，因为会调用复制构造函数
		Myclass2 obj1 = obj;
		return obj1;
	}
};
//在这个示例中，Myclass2的复制构造函数和赋值运算符被声明为私有的，并且没有提供定义
// 虽然友元函数copy_Myclass2和成员函数member_fun以及member_fun2试图复制对象，
// 但由于复制构造函数和赋值运算符不可访问，编译器会报错，无法链接成功。这进一步说明了禁止类复制的效果。
// ============================================================================
//要注意的是，在多文件编程时，如果写下Myclass2这样的类，编译阶段是不会报错的，因为编译器只会检查类的声明，而不会检查类的定义。
// 只有在链接阶段，编译器才会发现复制构造函数和赋值运算符没有定义，从而报错。因此，在多文件编程中，禁止类复制的效果可能会被延迟到链接阶段才显现出来。
//那有没有什么方法可以在编译阶段就发现这个问题，肯定是有的，uncopyable class呼之欲出

class Uncopyable {
private:
	Uncopyable(const Uncopyable&);
	Uncopyable& operator=(const Uncopyable&);
protected:
	Uncopyable() {};
	~Uncopyable() {};
};
class Myclass3 :private Uncopyable {

};
//在这个示例中，Uncopyable类被设计为一个不可复制的基类。它的复制构造函数和赋值运算符被声明为私有的，并且没有提供定义。
// Myclass3继承自Uncopyable，这意味着Myclass3也无法被复制。由于Uncopyable的复制构造函数和赋值运算符是私有的，任何试图复制Myclass3对象的操作都会在编译阶段报错。
// 通过使用Uncopyable作为基类，我们可以在编译阶段就禁止Myclass3的复制行为，从而避免了在链接阶段才发现复制构造函数和赋值运算符没有定义的问题。这是一种更为安全和有效的方式来禁止类的复制。
// 以后如果需要禁止类的复制行为，可以直接继承自Uncopyable类，不用在派生类中重复声明私有的复制构造函数和赋值运算符，这样可以提高代码的可维护性和可读性。
//现代C++11引入了删除函数的概念，可以直接使用delete关键字来禁止类的复制行为