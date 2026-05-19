#include<iostream>
#include"myhead.h"
int main() {
	int a = 1;
	int b = 4;
	int res = max<int>(a++, b++);
	std::cout << res << a << b << std::endl;
}

