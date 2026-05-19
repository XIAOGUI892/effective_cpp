#include<memory>
class Myclass{};
void func1(std::shared_ptr<Myclass>sp,int num);
int func2();
int main()
{
	func1(std::shared_ptr<Myclass>(new Myclass), func2());
	//问题出现，虽然我们用资源管理类来管理资源，但是仍然可能造成内存泄漏
	//核心原因在于当我们执行这段代码时，new这个动作一定发生在shared_ptr调用构造函数之前，但是fun2()和构造函数谁先执行就不好说了，如果new后fun2先执行并且发生了异常
	//那么就会造成内存泄漏
	//解决方法：以独立语句将new的对象放到资源管理对象中
	std::shared_ptr<Myclass>sp2(new Myclass);
	func1(sp2, func2());
	//编译器对于独立的语句没有顺序编排的自由，不会造成内存泄漏
}