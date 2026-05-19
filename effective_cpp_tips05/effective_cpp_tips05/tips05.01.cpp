#include<string>
class NameObject {
private:
	const int value;
	 std:: string& name;
public:
	NameObject(const int &v,std::string &name):value(v),name(name) {
	}
};

int main() {
	std::string name = "obj";
	NameObject obj1(1, name);
	std::string name2 = "obj2";
	NameObject obj2(2, name2);
	obj1 = obj2;//报错，编译器会自动生成一个赋值运算符函数，但是由于成员变量value是const类型，所以无法进行赋值操作，因此会报错。
}