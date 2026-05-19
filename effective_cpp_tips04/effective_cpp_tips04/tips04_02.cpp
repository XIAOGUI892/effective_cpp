#include <cstddef>
/*由其他文件提供：class FileSystem {
public:
	std::size_t numDisks()const;
};*/
extern FileSystem fileSystem;
class Directory {
public:
	Directory() {
		std::size_t disks = fileSystem.numDisks();
	}
};
int main() {
		Directory dir;
		//问题显现出来了，Directory的构造函数调用了fileSystem.numDisks()，但是fileSystem是一个外部变量，它可能还没有被初始化，这就导致了未定义行为。
		//如何解决这个问题呢？我们可以使用一个函数来返回fileSystem的引用，这样就可以保证在使用fileSystem之前它已经被初始化了。详见tips04_03.cpp
		return 0;
}