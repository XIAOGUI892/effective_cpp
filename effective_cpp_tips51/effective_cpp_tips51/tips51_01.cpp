#include<new>
#include<iostream>
void* operator new(std::size_t size) {
	if (size == 0)size == 1;//即使size为0，也要分配合理内存
	while (1) {
		void* ptr = malloc(size);
		if (ptr)return ptr;
		std::new_handler handler = std::get_new_handler();
		if (handler)(*handler)();
		else throw std::bad_alloc();
	}
}

void operator delete(void* ptr) {
	if (ptr == nullptr)return;
	free(ptr);//处理好特殊情况
}