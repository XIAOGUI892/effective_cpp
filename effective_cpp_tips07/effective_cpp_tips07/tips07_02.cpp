//#include<iostream>
//using namespace std;
//class Base1 {
//public:
//	virtual ~Base1() {
//		cout << "Base1 Destructor\n" << endl;
//	}
//	virtual void Bark() {
//		cout << "Base1 Bark\n" << endl;
//	}
//};
//class Base2 {
//public:
//virtual ~Base2() {
//	cout << "Base2 Destructor\n" << endl;
//}
//virtual void Bark() {
//		cout << "Base2 Bark\n" << endl;
//}	
//};
//class Derived :public Base1, public Base2 {
//private:
//	char* data;
//public:
//	Derived(size_t size) {
//		data = new char[size];
//	}
//	virtual ~Derived() {
//		delete[]data;
//		cout << "Derived Destructor\n" << endl;
//	}
//	virtual void Bark() {
//		cout << "Derived Bark\n" << endl;
//	}
//	virtual void Bark2() {
//		cout << "Derived Bark2\n" << endl;
//	}//派生类新增的成员函数，为虚函数，但查看类的内存布局，发现该类实例化的对象还是只有两个虚函数指针，分别指向Base1和Base2的虚函数表，而没有为Bark2新增一个虚函数指针
//	//那么该虚函数的地址存在哪里呢？存在Base1（主基类）的虚函数表中
//};
//int main() {
//	Base1* ptr = new Derived(10); // 传递一个size参数
//	delete ptr; // 通过基类指针删除派生对象，确保基类的析构函数是虚的
//
//	//其实当我们执行delete ptr时，我们调用的不是Base1的析构函数，而是Derived的析构函数，需要知道，编译器
//	//将我们的Derived析构函数给重写了，所以当我们执行delete ptr时，编译器会调用Derived的析构函数，
//	// 而Derived的析构函数又会调用Base1，Base2的析构函数，所以最终会正确地释放资源并输出相应的消息。
//
//	//如果不把Base1的析构函数声明为虚的，那么当我们执行delete ptr时，编译器会调用Base1的析构函数，
//	// 而不是Derived的析构函数，这样就会导致资源泄漏，因为Derived的析构函数没有被调用，data成员没有被释放。
//}