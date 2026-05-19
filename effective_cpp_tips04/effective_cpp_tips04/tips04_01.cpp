#include<string>
class ABEntry {
private:
	std::string name;
	std::string address;
	std::string phone;
	int numTimesConsulted;
public:
	ABEntry(const std::string& name, const std::string& address, const std::string& phone, int numTimesConsulted) {
		this->name = name;
		this->address = address;
		this->phone = phone;
		this->numTimesConsulted = numTimesConsulted;
		//注意，这是赋值，不是初始化
	}
	/*ABEntry(const std::string& name, const std::string& address, const std::string& phone, int numTimesConsulted):
	name(name),address(address),phone(phone),numTimesConsulted(numTimesConsulted) {
	}*/
	//注意，这是初始化，而不是赋值

	/*===============================================================*/
    //1.如果成员变量是const或者引用类型，那么必须使用初始化列表来初始化它们，因为它们不能被赋值。
	 
	//2.如果成员变量没有默认构造函数，那么必须使用初始化列表来初始化它们，否则编译器会报错。
	
	//3.如果成员变量是一个类类型，并且它有一个合适的构造函数，那么使用初始化列表可以避免不必要的默认构造和赋值操作，从而提高效率
	//这是因为不使用初始化列表会导致成员变量先被默认构造，然后再被赋值，而使用初始化列表则直接调用合适的构造函数来初始化成员变量。
	//对于内置类型的成员变量，使用初始化列表和在构造函数体内赋值的效果是一样的，但为了好看与一致性，还是使用成员初始化列表吧！dddddddddddddddddddd

};