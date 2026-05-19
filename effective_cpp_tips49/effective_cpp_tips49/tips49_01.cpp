//1. 问题引入：new 失败时发生了什么

#include <cstddef>
#include <malloc.h>
#include <new>
#include <iostream>
//int* p = new int[100000000000];  // 假设分配失败

//你的第一反应可能是"立刻抛 std::bad_alloc"。但实际上，operator new 内部的执行流程是这样的：
void* operator new(std::size_t size) {
	while (1) {
		void* ptr = malloc(size);
		if (ptr)return ptr;
		std::new_handler handler = std::get_new_handler();
		if (handler)(*handler)();
		else throw std::bad_alloc();
	}
}
/* 关键：不是一次分配失败就抛异常，而是进入一个循环——每次失败都调用 new-handler，期望它"想办法腾出内存"。*/
//只有当 new - handler 为空（nullptr）时，才最终抛异常。


//  2. 设定 new-handler：std::set_new_handler
void myNewHandler() {
	std::cerr << "内存分配失败，尝试补救...\n";
	// 策略：释放一些提前预留的内存
	// 或者：记日志、抛异常、abort()、卸载 new-handler...
}

/* 当你自己写一个 new-handler 时，它必须做以下几件事之一：

  ┌────────────────────────────────┬────────────────────────────────────────┐
  │              策略              │                  效果                  │
  ├────────────────────────────────┼────────────────────────────────────────┤
  │ 释放内存，让下一次尝试成功     │ 然后返回，operator new 再次调用 malloc │
  ├────────────────────────────────┼────────────────────────────────────────┤
  │ 安装另一个 handler             │ 下次循环用新的 handler                 │
  ├────────────────────────────────┼────────────────────────────────────────┤
  │ 卸载 handler（设为 nullptr）   │ 下次循环直接抛 bad_alloc               │
  ├────────────────────────────────┼────────────────────────────────────────┤
  │ 抛异常（bad_alloc 或其派生类） │ 跳出 operator new 循环                 │
  ├────────────────────────────────┼────────────────────────────────────────┤
  │ 不返回（abort() 或 exit()）    │ 直接终止程序                           │
  └────────────────────────────────┴────────────────────────────────────────┘*/
//test
int main() {
	std::set_new_handler(myNewHandler);    // 安装自定义 handler
	int* p = new int[1000000000000];//结果：循环打印 内存分配失败，尝试补救

}
