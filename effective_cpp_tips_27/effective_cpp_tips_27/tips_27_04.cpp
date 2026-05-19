//#include<iostream>
//#include<memory>
//#include<vector>
//class Window {
//public:
//	virtual  ~Window() = default;
//};
//
//
//class SpecialWindow :public Window {
//public:
//	void blink() {
//		std::cout << "blink blink!" << std::endl;
//	}
//};
//using VPM = std::vector<std::shared_ptr<SpecialWindow>>;
//
//int main() {
//	VPM winPtrs;
//	for (int i = 0; i < 5; i++) {
//		winPtrs.push_back(std::make_shared<SpecialWindow>());
//	}
//	for (int i = 0; i < 5; i++) {
//		SpecialWindow* ptr = winPtrs[i].get();
//		ptr->blink();
//		//不使用dynamic_cast，运行成本更高，但是不能在容器中存储指针指向各种Window派生类
//	}
//}
