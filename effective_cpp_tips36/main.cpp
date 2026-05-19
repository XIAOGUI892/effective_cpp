// Effective C++ 条款36：绝不重新定义继承来的非虚函数
// 三条展开规则：
//   1. 非虚函数 = 静态绑定 = 声明类型决定调用
//   2. 虚函数   = 动态绑定 = 实际对象类型决定调用
//   3. 重定义非虚函数 = 破坏 is-a 关系
#include "wrong.h"
#include "right.h"
#include "is_a_break.h"

int main() {
    demo_wrong();
    std::cout << std::endl;

    demo_right();
    std::cout << std::endl;

    demo_is_a();
    return 0;
}
