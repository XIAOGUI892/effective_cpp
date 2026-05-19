//#include<iostream>
//class Window {
//public:
//	virtual void onResize() {
//		windowSize *= 2;
//	}
//	Window(int size = 0) :windowSize(size) {}
//	int getwindowSize() {
//		return windowSize;
//	}
//private:
//	int windowSize;
//};
//
//class Specialwindow:public Window {
//public:
//	virtual void onResize() {
//		/*static_cast<Window*>(this)->onResize();*/ //无限递归，static_cast仅改变指针类型，不改变虚函数表，实现无限递归
//		static_cast<Window>(*this).onResize();
//		specialWindowSize *= 2;
//	}
//	Specialwindow(int size1,int size2):specialWindowSize(size1),Window(size2){}
//public:
//	int getSpecialWindowSize() {
//		return specialWindowSize;
//	}
//private:
//	int specialWindowSize;
//};
//int main() {
//	Specialwindow w(1, 2);
//	std::cout << w.getwindowSize() << " " << w.getSpecialWindowSize()<<std::endl;
//	w.onResize();
//	std::cout << w.getwindowSize() << " " << w.getSpecialWindowSize()<<std::endl;
//	return 0;
//	//问题：基类的window的windowSize的更改未成功，原因是tatic_cast<Window>(*this)创建了一个临时对象，不是Specialwindow内部的Window对象
//	/*正确的做法是：virtual void onResize() {
//						Window::onResize();
//						specialWindowSize *= 2;
//					}*/
//}