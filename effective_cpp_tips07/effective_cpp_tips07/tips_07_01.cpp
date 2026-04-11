//#include<string>
//class SpecialString :public std::string {
//private:
//	char* data;
//public:
//	SpecialString() {
//		data = new char[100];
//	}
//};
//
//int main() {
//	std::string* ps;
//	SpecialString* pss = new SpecialString();
//	ps = pss;
//	// 不要通过 std::string*（基类指针）来删除派生对象，因为 std::string 的析构函数不是虚的，
//	// 这样会导致未定义行为。改为通过派生类指针删除：
//	delete pss;	
//}