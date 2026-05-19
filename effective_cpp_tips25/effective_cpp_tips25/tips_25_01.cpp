#include<algorithm>
class WidgetImpl{};
class Widget {
public:
	Widget(const Widget& other);
	Widget& operator=(const Widget& other) {
		*pImpl = *(other.pImpl);
		return *this;
	}
private:
	WidgetImpl* pImpl;

};
//在std::命名空间下有下面的交换函数
template<typename T>
void swap(T& a, T& b) {
	T tmp = a;
	a = b;
	b = a;
}
//问题在于，如果我们想交换两个Widget对象，本质上只要交换pImpl所指向的对象就行了
//std交换函数不仅赋值三个Widget对象，还赋值三个WidgetImpl对象
//效率好低！！！
//解决方法：特化