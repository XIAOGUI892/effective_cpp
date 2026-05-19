#include <memory>
#include <iostream>
#include<vector>
int main() {
	std::shared_ptr<int>ptr(new int[1024]);
	//析构时调用的是delete操作，而不是delete[]，这会导致内存泄漏
	//要注意vector与string几乎可以替代动态分配而来的数组了，不需要使用智能指针来管理动态分配的数组了
}