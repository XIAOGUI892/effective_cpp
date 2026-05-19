//如何避免二义性：虚继承
class Base {
public:
    Base(int x):x_(x){}
private:
    int x_;
};

class A : virtual public Base {
private:
    int a_;
public:
    A(int a):a_(a),Base(a){}
};

class B : virtual public Base {
private:
    int b_;
public:
    B(int b):b_(b),Base(b){}
};

class Final : public A, public B {
public:
    Final(int f,int e):A(f),B(f),Base(e){}
private:
    int f_;
};



//菱形继承需要注意：最终派生类负责虚基类的初始化，中间类对虚基类的初始化被编译器省略
//但这带来了一个问题：我们在写最终派生类的构造函数时要找到虚基类，承担其初始化责任，无论继承体系多么深
//而且虚继承有一定的时间空间开销
//我们希望虚基类只做接口，不保存数据，解决了最终派生类的构造函数时要找到虚基类，承担其初始化责任的问题，如下



/*class Base {
public:
    Base() {}
private:
};

class A : virtual public Base {
private:
    int a_;
public:
    A(int a) :a_(a) {}
};

class B : virtual public Base {
private:
    int b_;
public:
    B(int b) :b_(b) {}
};

class Final : public A, public B {
public:
    Final(int f, int e) :A(f), B(f) {}
private:
    int f_;
};*/