#include<iostream>
#include<string>
#include<exception>
class Textbook {
private:
	std::string text;
public:
	Textbook(const std::string &t):text(t){}
	const char& operator[](std::size_t pos)const {
		if(pos>=text.size()||pos<0) {
			throw std::out_of_range("pos is out of range");
		}
		return text[pos];
	}
	char &operator[](std::size_t pos) {
		if(pos>=text.size()||pos<0) {
			throw std::out_of_range("pos is out of range");
		}
		return text[pos];
	}
};

int main() {
	Textbook book("Effective C++");
	std::cout<<book[0]<<std::endl;
	book[0]='e';//编译器认为，const成员函数只要不在函数内部修改成员变量，就是合法的；
	std::cout<<book[0]<<std::endl;
	return 0;
}