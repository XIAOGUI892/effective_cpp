#include<memory>
#include<mutex>
struct Class {
	std::shared_ptr<int>sp;
	int count;
};
class MyClass {
public:
	void change();
private:
	std::mutex mtx_;
	std::shared_ptr<Class>ptr;
};
void MyClass::change() {
	using std::swap;
	std::lock_guard<std::mutex>lock(mtx_);
	std::shared_ptr<Class>ptr1(new Class);
	ptr1->sp = ptr->sp;
	ptr1 -> count = ptr->count;
	swap(ptr1, ptr);
}
//异常安全：
//用一个副本保存原始资源，如果保存的过程中抛出异常，原始资源不会被破坏
//我们用一个异常安全的swap函数，保证不抛出异常，关于swap函数，详见tips_25
