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

};
// 这个SharedPtr类实现了基本的智能指针功能，包括引用计数、资源管理 ，通过使用SharedPtr，可以更安全地管理动态分配的资源，避免内存泄漏和资源管理错误。