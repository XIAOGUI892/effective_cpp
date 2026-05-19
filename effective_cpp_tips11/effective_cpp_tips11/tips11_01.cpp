#include<cstring>
#include<iostream>
using namespace std;
class Myclass {
private:
	char* data;
public:
	Myclass(const char* str) {
		data = new char[strlen(str) + 1];
		strcpy(data, str);
	}
	~Myclass() {
		delete[] data;
	}
	Myclass& operator=(const Myclass& other) {
		if (this != &other) {
			delete[]data;
			data = new char[strlen(other.data) + 1];//问题出现：如果new出现异常，但data已经delete了，不满足异常安全保证
			strcpy(data, other.data);
		}
		return *this;
	}
};
//改进方法
Myclass& Myclass::operator=(const Myclass& other) {
	char* temp = data;
	data = new char[strlen(other.data) + 1];
	strcpy(data, other.data);
	delete[]temp;
	return *this;
	//好处：既解决了自我赋值的问题，又在new抛出异常的时候，data还没有被修改，满足了异常安全保证

//或者使用copy-and-swap 技术
	Myclass& Myclass::operator=(Myclass other) {
		swap(data, other.data);//要保证swap函数满足noexcept，详见条款29
		return *this;
	}
}