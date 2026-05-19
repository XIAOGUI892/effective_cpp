#include<string>
#include"Date.h"
#include"Address.h"
class Person {
private:
	std:: string name;
	Data theBirth;
	Address theAddress;
};
//问题来了：如果theBirth或theAddress头文件发生改变，那么Person头文件会受到影响，所有包含Person.h的.cpp文件都要重新编译，浪费时间