//#include<iostream>
//#include<memory>
//#include<vector>
//class Window {
//public:
//    virtual  ~Window() = default;
//};    
//
//using VPM = std::vector<std::shared_ptr<Window>>;
//
//class SpecialWindow :public Window{
//public:
//	void blink() {
//		std::cout << "blink blink!" << std::endl;
//	}
//};
//int main() {
//	VPM winPtrs;
//	for (int i = 0; i < 5; i++) {
//		winPtrs.push_back(std::make_shared<Window>());
//	}
//	for (int i = 0; i < 5; i++) {
//		SpecialWindow* ptr = dynamic_cast<SpecialWindow*>(winPtrs[i].get());
//		ptr->blink();
//		//问题：多次dynamic_cast类型转换开销很大
//		//解决方法看tips_27_04 tips_27_05
//	}
//}
