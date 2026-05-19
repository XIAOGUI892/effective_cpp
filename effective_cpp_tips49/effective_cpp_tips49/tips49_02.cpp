//第一个矛盾：类需要不同的 new - handler
//std::set_new_handler 设置的是全局 handler。但不同类在分配失败时需要不同的处理：
//不同的类在内存发票失败时可能需要做出不同的反应

//方案：类专属 new-handler + 类专属 operator new
#include <new>
#include <iostream>
class Image {
private:
	static std::new_handler currentHandler;
public:
	static std::new_handler set_new_handler(std::new_handler h) {
		std::new_handler oldHandler = currentHandler;
		currentHandler = h;
		return oldHandler;
	}

	static void* operator new(std::size_t size) {
		std::new_handler globalHandler = std::set_new_handler(currentHandler);
		void* memory;
		try {
			memory = ::operator new(size);
		}
		catch (...) {
			std::set_new_handler(globalHandler);
			throw;
		}
		std::set_new_handler(globalHandler);
		return memory;

	}
};

std::new_handler Image::currentHandler = nullptr;

void imageOutOfMemoryHandler() noexcept{
	std::cerr << "error" << std::endl;
}
//调用时
int main() {
	Image::set_new_handler(imageOutOfMemoryHandler);
	Image* ptr = new Image;
}