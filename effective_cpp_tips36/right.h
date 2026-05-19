// 条款36：绝不重新定义继承来的非虚函数
// 本文件演示【正确做法】
#pragma once
#include <iostream>

// 基类 — 虚函数代表"允许子类改变的行为"
struct VirtBase {
    virtual void f() { std::cout << "  VirtBase::f" << std::endl; }
    virtual ~VirtBase() = default;
};

// 子类 — 正确 override，而非重新定义
struct VirtDerived : VirtBase {
    void f() override { std::cout << "  VirtDerived::f" << std::endl; }
};

inline void demo_right() {
    std::cout << "=== 正确演示：虚函数 + override ===" << std::endl;

    VirtDerived  d;
    VirtBase*    vb = &d;
    VirtDerived* vd = &d;

    std::cout << "基类指针调用 → ";
    vb->f();   // 动态绑定 → 实际对象 VirtDerived → VirtDerived::f

    std::cout << "子类指针调用 → ";
    vd->f();   // 动态绑定 → 实际对象 VirtDerived → VirtDerived::f
}
