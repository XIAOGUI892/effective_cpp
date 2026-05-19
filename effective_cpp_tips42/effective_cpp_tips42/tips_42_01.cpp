//关于typename双重含义
// 
// 
//第一重意义：声明模板参数
//和 class 完全等价，声明模板类型参数：
//template<typename T>    typename = "T 是任意类型"
//template<class T>       class 也可以，一模一样
//无任何区别。之所以保留两种写法，是历史原因——class 先引入，后来才加了 typename 避免语义混淆。


//=====================================================================================================

//第二重意义：
//写模板代码时，模板参数 T 内部定义的东西叫嵌套从属名称。编译器默认不知道它是类型还是变量 / 函数：
//
//template<typename T>
//void foo() {
//    T::iterator* x;   // 这行有歧义！
//}
//
//编译器如何解读 T::iterator* x？
//
//- 如果 T::iterator 是类型 → 声明一个指针 x
//- 如果 T::iterator 是静态成员变量 → 就是乘法运算
//
//编译器默认假定嵌套从属名称不是类型。所以上面那行会被当成乘法——编译失败。
//
//解决方法：用 typename 告诉编译器"这是个类型"：
//
//template<typename T>
//void foo() {
//    typename T::iterator* x;   // 明确：iterator 是类型，x 是指针
//}

#include <vector>
#include<iostream>
template<typename Container>
void printFirst(const Container& c) {
    if (c.empty()) return;

    typename Container::const_iterator it = c.begin();  // 必须加 typename
    // 没有 typename → 编译器认为 const_iterator 是静态成员，编译报错

    std::cout << *it;
}
int main() {
    std::vector<int> v{1, 2, 3};
    printFirst(v);  // 输出 1
    return 0;
}   