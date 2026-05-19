template<typename T>
class SmartPtr {
private:
	T* ptr;
public:
	template<typename U>
	SmartPtr(U* p) { ptr = p; }

	SmartPtr(T* p) { ptr = p; }

	template<typename U>
	SmartPtr(SmartPtr<U>& sp):ptr(sp.get()) 
	{ }
	// 对任何类型 U，只要 U* 能隐式转 T*，就允许 SmartPtr<U> 转 SmartPtr<T>
	//这就是运用成员函数模板接受所有兼容类型
	template<typename U>
	SmartPtr(const SmartPtr<U>&sp):ptr(sp.get()){}

	template<typename U>
	SmartPtr& operator=(const SmartPtr<U>& sp) {
		ptr = sp.get();
	}

	SmartPtr& operator=(const SmartPtr& sp) {
		ptr = sp.get();
	}

	SmartPtr(const SmartPtr& sp):ptr(sp.get()){ }
	   //  重要：模板构造函数不会阻止编译器生成默认拷贝构造！
		     // 如果需要控制普通拷贝的行为，必须显式声明 =default 或自己写

	T* get()const {
		return ptr;
	}

};
int main() {
	SmartPtr<int> const sp0(new int(10));
	SmartPtr<const int> const sp1 = sp0;
}