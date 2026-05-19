#include<iostream>
#include<string>
class Textbook {
private:
	std::string text;
public:
	Textbook(const std::string& t) :text(t) {}
	const char& operator[](std::size_t pos)const {
		if (pos >= text.size() || pos < 0) {
			throw std::out_of_range("pos is out of range");
		}
		return text[pos];
	}
	/*char& operator[](std::size_t pos) {
		if (pos >= text.size() || pos < 0) {
			throw std::out_of_range("pos is out of range");
		}
		return text[pos];
	}*/
	//用类型转换和const函数实现重载
	char& operator[](std::size_t pos) {
		return const_cast<char&>(static_cast<const Textbook&>(*this)[pos]);
		//static_cast实现了从Textbook到const Textbook的转换，const_cast实现了从const char&到char&的转换,因为char&无法接受const char&的返回值，
		// 所以需要使用const_cast进行类型转换，以满足函数重载的需求。
	}
	//======================================================================================
	//1.思考：为什么不用类型转换和non_const函数实现重载，因为non_const函数不承诺不修改成员变量，与我们要实现的non_const函数的功能不符，
	// 可能会导致编译器无法正确推断函数调用的版本，从而引发编译错误或运行时错误。
	//=======================================================================================
	//2.思考：为什么不用const_cast实现非const对象到const对象的转换
	//const_cast 只是 添加const限定符 ，但没有改变对象的 基本类型，对象仍然是 Textbook& 类型（只是添加了const限定）
	//编译器仍然认为它是非const对象，所以仍然调用非const版本
	//static_cast 进行 真正的类型转换
	//- 将 Textbook& 转换为 const Textbook&
	//改变了对象的 基本类型 ，而不仅仅是添加限定符
	//编译器认为它是const对象，所以调用const版本
};

int main() {
	Textbook book("Effective C++");
	std::cout << book[0] << std::endl;
}