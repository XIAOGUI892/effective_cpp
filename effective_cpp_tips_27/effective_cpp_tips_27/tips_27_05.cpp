#include<iostream>
#include<memory>
#include<vector>
class Window {
public:
	virtual  ~Window() = default;
	virtual void blink() {}
};


class SpecialWindow :public Window {
public:
	void blink() {
		std::cout << "blink blink!" << std::endl;
	}
};
using VPM = std::vector<std::shared_ptr<Window>>;

int main() {
	VPM winPtrs;
	for (int i = 0; i < 5; i++) {
		winPtrs.push_back(std::make_shared<Window>());
	}
	for (int i = 0; i < 5; i++) {
		Window* ptr = winPtrs[i].get();
		ptr->blink();
	}
	//在base class 中提供虚函数做你想让派生类做的事情，解决了不能在容器中存储指针指向各种Window派生类的问题
	//条款34告诉我们，这样实现代码可能不是个好主意，但解决上述问题还不错！！！
}
