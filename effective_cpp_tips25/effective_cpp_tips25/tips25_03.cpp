//现在我们提供一个偏特化的模板
namespace WidgetStuff {
	template<typename T>
	class Widget {
	public:
		void swap(Widget& other) {
			using std::swap;
			swap(ptr, other.ptr);
		}
	private:
		T ptr;
	};
	template<typename T>
	void swap(Widget<T>& a, Widget<T>& b) {
		a.swap(b);
	}
}
template<typename T>
void dosomething(T &a,T &b) {
	using std::swap;
	swap(a, b);
 }
//将我们的swap函数与类放在同一个命名空间，这样在调用swap函数时先查找global命名空间或类所在的命名空间的swap函数
// //如果没有，由于我们声明了std::swap函数，所以编译器会查看std命名空间的swap函数，且优先调用全特化的版本
//这样，我们总能匹配到最佳的swap()函数