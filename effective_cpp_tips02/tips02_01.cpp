#include<iostream>
class CostEstimate {
public:
	const int cnt;//或者是const int cnt  = 0;0相当于cnt的默认值
public:
	CostEstimate(int c) :cnt(c) {}//注意，这里必须要初始化cnt，不能在构造函数体内赋值,当然，如果再声明时就初始化cnt，构造函数就不需要初始化列表了
	int getCount() const { return cnt; }
};
int main(){
	CostEstimate ce(10);
	std::cout << ce.cnt;
	// ce.cnt = 20; // error: assignment of read-only member ‘CostEstimate::cnt’
	return 0;
}