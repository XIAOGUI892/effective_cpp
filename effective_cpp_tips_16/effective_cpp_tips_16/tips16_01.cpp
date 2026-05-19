#include<string>
int main() {
	std::string* ptr = new std::string[10];
	//delete ptr;//问题出现：delete与new不匹配
	delete[]ptr;//[]告诉delete要删除的是一个数组

	std::string* ptr = new std::string;
	delete ptr;//这就没错了

	//记住delete与new要配对，这也告诉我们，在资源管理类中要注意，多个构造函数的new要一致，因为析构函数只有一种形式
	//智能指针的delete没有[]，很大程度上是因为vector与string可以很好的管理动态数组
}