class Investment{};
Investment* createInvestment(){
	return new Investment;
}
void fun() {
		Investment* p = createInvestment();
	// do something with p
		delete p;
}
//问题出现在于，如果在do something with p的过程中发生了异常，或者程序在某个地方意外地返回了，那么p就永远不会被delete掉，造成内存泄漏
// 这就引出了以对象的形式管理资源的概念，通常被称为RAII（Resource Acquisition Is Initialization）。
// 通过将资源的获取和释放封装在一个对象中，我们可以确保资源在不再需要时被正确地释放，无论是正常执行还是发生异常。
//1。如果return，离开作用域，析构函数会被调用，资源会被释放
//2.如果发生异常，离开作用域，析构函数会被调用，资源会被释放

//智能指针就是RAII的一种实现，它通过重载运算符来管理资源的生命周期，确保资源在不再需要时被正确地释放。
// 使用智能指针可以大大减少内存泄漏和资源管理错误的风险，提高代码的安全性和可维护性。
//当然，析构函数也可能抛出异常，但这是条款8讨论的事情，下面来看RAII的示例