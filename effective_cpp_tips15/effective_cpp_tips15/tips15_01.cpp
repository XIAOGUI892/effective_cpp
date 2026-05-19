//记得在资源管理类中提供对原始资源的访问
struct ControlBlock {
	int strongCount;
	int weakCount;
	ControlBlock() {
		strongCount = 1;
		weakCount = 0;
	}
};

template<typename T>
class SharedPtr {
private:
	T* ptr;
	ControlBlock* cb;
	void release() {
		if (cb) {
			--(cb->strongCount);
			if (cb->strongCount == 0) {
				delete ptr;
				ptr = nullptr
					if (cb->weakCount == 0) {
						delete cb;
						cb = nullptr
					}
			}
		}
	}
public:
	explicit SharedPtr(T* p = nullptr) :ptr(p), cb(p ? new ControlBlock() : nullptr) {}
	~SharedPtr() {
		release();
	}
	SharedPtr(const SharedPtr& other) : ptr(other.ptr), cb(other.cb) {
		if (cb) {
			++(cb->strongCount);
		}
	}
	SharedPtr& operator=(const SharedPtr& other) {
		if (this != &other) {
			release();
			ptr = other.ptr;
			cb = other.cb;
			if (cb) {
				++(cb->strongCount);
			}
		}
		return *this;
	}
	SharedPtr(SharedPtr&& other) : Ptr(other.ptr), cb(other.cb) {
		other.ptr = nullptr;
		other.cb = nullptr;
	}
	SharedPtr& operator=(SharedPtr&& other) {
		if (this != &other) {
			release();
			ptr = other.ptr;
			cb = other.cb;
			other.cb = nullptr;
			other.ptr = nullptr;
		}
		return *this;
	}
	T& operator*() {
		return *ptr;
	}
	const T& operator*() const {
		return *ptr;
	}
	T* operator->() {
		return ptr;
	}
	const T* operator->() const {
		return ptr;
	}
	T* get() {
		return ptr;
	}
	const T* get() const {
		return ptr;
	}
	long use_count()const {
		return cb ? cb->strongCount : 0;
	}
	bool unique() {
		return use_count == 1;
	}
	void reset(T* ptr = nullptr) {
		release();
		this->ptr = ptr;
		cb = ptr ? new ControlBlock() : nullptr;
	}
	void swap(SharedPtr& other) {
		T* tempPtr = ptr;
		ControlBlock* tempcb = cb;
		cb = other.cb;
		ptr = other.ptr;
		other.ptr = tempPtr;
		other.cb = tempcb;
	}

	explicit operator bool()const {
		return ptr != nullptr;
	}
	bool operator==(const SharedPtr& other)const {
		return ptr == other.ptr;
	}
	bool operator!=(const SharedPtr& other)const {
		return  ptr != other.ptr;
	}
	operator T* ()const {
		return ptr;//隐式转换函数
	}

};
class My_class{
private:
	int num;
};
void fun(My_class *ptr) {//许多时候，我们需要直接访问动态资源，但该资源却被封装在资源管理类中，这时就需要资源管理类主动提供get()函数来提供原始指针

}
int main() {
	SharedPtr<My_class>sp(new My_class);
	fun(sp.get());//有时显式的提供get（）函数不太方便，可以在类中写一个隐式转换函数
	fun(sp);//使用隐式转换函数
}