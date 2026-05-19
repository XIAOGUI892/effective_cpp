/*  voiddoProcessing(Widget& w) {
	if (w.size() > 0 && w != someNastyWidget) {
		Widget temp(w);
		temp.normalize();
		temp.swap();
	}
}*/
//w是Widget类型，w必须支持size(),operator!=, copy constructor, normalize(), swap()等操作
//我们可以在源码中（Widget）的类实现中找到这些接口，所以这是显示接口
//显示接口在编译器完成检查，如果不满足条件，编译器会报错，提示我们缺少接口或者接口不匹配1


template<typename T>
void doProcessing(T& w) {
	if (w.size() > 0 && w != someNastyWidget) {
		Widget temp(w);
		temp.normalize();
		temp.swap();
	}
}

//上面是一个模板函数，T是一个模板参数，可以是任何类型
//这是隐式接口
////                              约束1: T必须有 size()，返回值可与int比较
//T copy(obj);                 约束2: T必须可拷贝构造
//copy.normalize();            约束3: T必须有 normalize()
//copy.swap(obj);              约束4: T必须有 swap()，且参7数类型匹配

//  隐式接口不是写在代码里的声明，而是由模板函数体中的每一条语句共同定义的、对类型参数的隐含约束集合。
//这些约束没有写在 class T : public Base 里，而是散落在函数体各处的隐式接口
//显示接口在编译器完成检查，如果不满足条件，编译器会报错，提示我们缺少接口或者接口不匹配1
// 显示接口的多态在编译器完成，通过T的具现化和函数重载解析实现多态
//