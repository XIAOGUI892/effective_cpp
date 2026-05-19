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
	delete ptr;
	//想一想，如果基类的析构函数不是虚函数，会发生什么
	//最容易想到的：派生类的动态资源泄漏
	//不容易想到的是Derived对象本身的内存也会泄漏
	//为什么???
	/* // 1. Base destructor —— 不管释放内存
  Derived::~Derived() {           // ← 这个没有 operator delete 调用
      blob_.cleanup();            // 用户的析构体
      Base::~Base();              // 基类析构
  }

  // 2. Deleting destructor —— 析构 + 释放
  Derived::~Derived_scalar_deleting() {
      Derived::~Derived();        // 先跑上面的
      operator delete(this, sizeof(Derived));   // ← 在这里！写死 sizeof(Derived)
  }*/


	//这是编译器为我们自动生成的两份析构函数
	//当我们delete时，调用的是版本2
	//如果没有虚析构函数，delete时调用的就是
	/*Base::~Base_scalar_deleting() {
      Base::~Base();        // 先跑上面的
      operator delete(this, sizeof(Base));   
	  释放的资源大小不是sizeof(derived),而是sizeof(Base)
	  //资源泄漏
  }*/
}

