#include<iostream>
class Base {
public:
    int x_;
};

class A :  public Base {
public:
    int a_;
};

class B :  public Base {
public:
    int b_;
};

class Final : public A, public B {
public:
    int f_;
};
//多重继承
int main() {
    Final f;
    std::cout << f.x_ << std::endl;
    //问题来了：x_是A的还是B的
    //可以用作用域指定：f.A::x_
}