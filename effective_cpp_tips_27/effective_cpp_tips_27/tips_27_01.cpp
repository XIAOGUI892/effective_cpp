//#include<iostream>
//class Base {
//private:
//	int Base_data;
//public:
//	int getBaseData() {
//		return Base_data;
//	}
//	Base(int data = 0):Base_data(data){}
//};
//class Derived :public Base {
//private:
//	int Derived_data;
//public:
//	int getDerivedBase() {
//		return Derived_data;
//	}
//	Derived(int data1,int data2):Base(data1),Derived_data(data2){}
//};
//
//int main() {
//	Derived d1(1,2);
//	int* ptr = reinterpret_cast<int*>(&d1);
//	int* ptr2 = ptr + 1;
//	std::cout << *ptr << " " << *ptr2;
//	//成功打印出1 2
//	//但问题在于，reinterpret_cast类型转换实现动作及结果取决于编译器，不具有可移植性
//	//我们通过d1的内存布局来获取data值，放在另外一个平台上未必可行，因为对象的内存布局及地址计算方式随编译器的不同而不同
//}