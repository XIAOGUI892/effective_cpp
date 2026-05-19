// 条款36：绝不重新定义继承来的非虚函数
// 本文件演示【错误做法】
#pragma once
#include <iostream>

// 基类 — 非虚函数代表"不变的承诺"
struct PtrBase {
    void f() { std::cout << "  PtrBase::f — 我是唯一的 f" << std::endl; }
};

// 子类 — 错误地重新定义了 f
struct PtrDerived : PtrBase {
    void f() { std::cout << "  PtrDerived::f — 我是篡改版 f" << std::endl; }
};

inline void demo_wrong() {
    std::cout << "=== 错误演示：重定义非虚函数 ===" << std::endl;

    PtrDerived  d;
    PtrBase*    pb = &d;
    PtrDerived* pd = &d;

    std::cout << "基类指针调用 → ";
    pb->f();   // 静态绑定 → 声明类型 PtrBase → PtrBase::f

    std::cout << "子类指针调用 → ";
    pd->f();   // 静态绑定 → 声明类型 PtrDerived → PtrDerived::f
}
