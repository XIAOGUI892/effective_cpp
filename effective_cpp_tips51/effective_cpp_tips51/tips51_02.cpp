#include<iostream>
class Base {
public:
	virtual ~Base() {
		std::cout << "Base creating!" << std::endl;
	}

	static void* operator new(std::size_t size) {
		if (size != sizeof(Base))return ::operator new(size);
		std::new_handler globalHandler = std::set_new_handler(handler_);
		void* ptr;
		try {
			 ptr = ::operator new(size);
		}
		catch (...) {
			std::set_new_handler(globalHandler);
			throw;
		}
		std::set_new_handler(globalHandler);
		return ptr;
	}

	static std::new_handler set_new_handler(std::new_handler handler) {
		std::new_handler old = handler_;
		handler_ = handler;
		return old;
	}

	void operator delete(void* ptr, size_t size) {
		if (size != sizeof(Base)) {
			::operator delete(ptr);
			return;
		}
		if (ptr == nullptr)return;
		free(ptr);
	}

private:
	static std::new_handler handler_;
};

class Derived :public Base {

};

int main() {
	Base* ptr = new Derived();
	//像条款50说的我们有时会为类定制new，但如果基类被继承，派生类没有定制自己的new，那么派生类new时调用的是基类的new，
	//解决方法：调用默认new
	//if (size != sizeof(Base))return ::operator new(size);
	//同样 delete需要配对的if (size != sizeof(Base)) {
								//::operator delete(ptr);
								//return;
								//}
}

