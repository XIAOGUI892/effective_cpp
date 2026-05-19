#include <algorithm>
class WidgetImpl {};
class Widget {
public:
	Widget(const Widget& other);
	void swap(Widget& other) {
		using std::swap;
		swap(pImpl, other.pImpl);
	}
private:
	WidgetImpl* pImpl;
};

namespace std {
	template<>
	void swap<Widget>(Widget &a,Widget& b){
		a.swap(b);
	}
}