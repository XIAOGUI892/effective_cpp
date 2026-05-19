#include<mutex>
class MyClass {
public:
	void change();
private:
	std::mutex mtx_;
	int* ptr;
	int count;
};
void MyClass::change() {
	mtx_.lock();
	delete ptr;
	count++;
	ptr = new int[10];
	mtx_.unlock();
}
//这个函数没有异常安全性可言
//为什么：
//1.如果new抛出异常，ptr已经被delete了，原始数据被破坏，
//2.mtx_.unlock()无法执行，其他函数再也无法拿到锁
//3.修改失败，修改次数count应该保持原来的大小不变，但这里却++
//解决方法见tips_29_02