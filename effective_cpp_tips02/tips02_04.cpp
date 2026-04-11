#define MAX_NUM(a,b) (a>b ? a : b)
#include <iostream>
int main() {
	int a = 3;
	int b = 0;
	int res = MAX_NUM(a++, b++);//尝试调试，发现直接跳过去，难以进行单步调试，说明宏定义在预处理阶段就被替换了，编译器无法识别它的结构，所以无法进行单步调试，这也是宏定义的一个缺点，解决方法：使用内联函数或者constexpr函数来替代宏定义，这样就可以进行单步调试了
	std::cout << res << std::endl;
	std::cout << a << b << std::endl;//输出结果是3 4 1，说明a++和b++被执行了两次，导致a和b的值都增加了1，这就是宏定义的副作用,解决方法：使用内联函数或者constexpr函数来替代宏定义，这样就不会有副作用了
}