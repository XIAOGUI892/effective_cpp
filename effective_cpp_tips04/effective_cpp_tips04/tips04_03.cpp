
#include <cstddef>
/*由其他文件提供：class FileSystem {
public:
	std::size_t numDisks()const;
};*/

FileSystem& tfs() {
	static FileSystem fs;
	return fs;
}
class Directory {
public:
	Directory() {
		std::size_t disks = tfs().numDisks();
	}
};
//解决了上一个代码片段中的问题，我们使用了一个函数tfs()来返回fileSystem的引用，
// 这样就可以保证在使用fileSystem之前它已经被初始化了。这个函数内部定义了一个静态变量fs，
// 它会在第一次调用tfs()时被初始化，并且在程序的整个生命周期内保持有效。这样我们就避免了未定义行为，并且保证了Directory的构造函数能够正确地调用fileSystem.numDisks()。