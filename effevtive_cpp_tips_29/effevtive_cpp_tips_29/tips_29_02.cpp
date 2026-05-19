#include<memory>
#include<mutex>
class MyClass {
public:
	void change();
private:
	std::mutex mtx_;
	std::shared_ptr<int>ptr;
	int count;
};
void MyClass::change() {
	std::lock_guard<std::mutex>lock(mtx_);
	ptr.reset(new int[20]);
	count++;
	//具备异常安全性：
	//1.RAII思想：运用对象管理资源，保证了锁的正常释放
	//将count放在reset之后，如果new int[20]失败了，不会进入到reset函数，原始资源的安全性得到保障
	//3.new int[20]失败抛出异常，count++不会进行，保证了计数的正确性
}
