#include<string>
#include <stdexcept>
const  int  MiniPassWordLength = 10;
std::string encryptPassWord1(const std::string& password) {
	std::string encrypted;
	if (password.length() < MiniPassWordLength) {
		throw std::logic_error("password is too short!");
	}
	encrypted = password+"dcd";
	return encrypted;
	//问题：当我们写下一个对象时，就要考虑它的构造和析构成本
	//如果抛出异常，那么encrypted就要析构，并且异常抛出之前就已经调用了构造函数
	// //所以最好在异常后在声明entrypted
	//如果不抛出异常，std::string encrypted:调用了默认构造函数，没有嵌入我们想要的内容,还要再调用一次赋值函数
	//这是不必要的函数调用开销
}

//这是改进
std::string encryptedPassWord2(const std::string password) {
	if (password.length() < MiniPassWordLength) {
		throw std::logic_error("password is too short!");
	}
	std::string encrypted = password + "cdv";
	return encrypted;
}