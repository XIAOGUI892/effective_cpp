// 条款36：绝不重新定义继承来的非虚函数
// 本文件演示【is-a 关系如何被破坏】
#pragma once
#include <iostream>
#include <string>

// 基类承诺：所有鸟 flap() 都是"扇翅膀飞行"
struct Bird {
    void flap() { std::cout << "  每秒扇翅膀 3 下" << std::endl; }
    std::string name() { return "鸟"; }
};

// 企鹅 IS-A 鸟，却推翻了 flap 的承诺
struct Penguin : Bird {
    void flap() { std::cout << "  企鹅不会飞" << std::endl; }
    std::string name() { return "企鹅"; }
};

inline void demo_is_a() {
    std::cout << "=== is-a 关系被破坏 ===" << std::endl;

    Penguin  p;
    Bird&    b  = p;   // 没问题：企鹅 IS-A 鸟
    Penguin& p2 = p;

    std::cout << "同一个对象，通过 Bird& 看到的行为：" << std::endl;
    b.flap();          // 静态绑定 → Bird::flap → "扇翅膀"

    std::cout << "同一个对象，通过 Penguin& 看到的行为：" << std::endl;
    p2.flap();         // 静态绑定 → Penguin::flap → "不会飞"

    std::cout << "→ 矛盾：同一个企鹅对象，既是鸟又不会飞"
              << std::endl;
}
