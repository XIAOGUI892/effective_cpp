//非虚函数表达的是：所有子类的这个行为必须完全一致。它不应该随子类而改变。这是"不变性"的保证。

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

class Person {
public:
    Person(std::string name, int id) : name_(std::move(name)), id_(id) {}
    // 非虚函数：所有"人"获取名字的方式都一样
    const std::string& getName() const { return name_; }
    int getId() const { return id_; }
    // 虚函数：不同职业打招呼方式不同
    virtual std::string greet() const = 0;

    virtual ~Person() = default;

private:
    std::string name_;
    int id_;
};

class Student : public Person {
public:
    using Person::Person;
    std::string greet() const override {
        return "老师好！我是学生 " + getName();
    }
};

class Professor : public Person {
public:
    using Person::Person;
    std::string greet() const override {
        return "同学们好！我是教授 " + getName();
    }
};

int main() {
    Student s("张三", 1001);
    Professor p("李四", 2001);

    // getName() 是非虚函数，行为完全一致、可预测
    std::cout << s.getName() << " 打招呼: " << s.greet() << "\n";
    std::cout << p.getName() << " 打招呼: " << p.greet() << "\n";
}

//关键原则：如果函数的行为在所有子类中不应该有差异，就声明为非虚函数。