#include <iostream>
#include <string>

// 抽象基类：所有飞行器都必须能起飞
class Aircraft {
public:
    // 纯虚函数：只声明接口，不提供实现
    virtual void takeOff() = 0;          // 接口契約：你必须会起飞
    virtual double fuelConsumption() const = 0;
    virtual ~Aircraft() = default;
};

// 直升机用自己的方式实现起飞
class Helicopter : public Aircraft {
public:
    void takeOff() override {
        std::cout << "直升机：垂直升空，旋翼加速至 400 RPM\n";
    }
    double fuelConsumption() const override {
        return 320.0; // L/h
    }
};

// 固定翼飞机用完全不同的方式实现起飞
class FixedWing : public Aircraft {
public:
    void takeOff() override {
        std::cout << "固定翼飞机：跑道滑跑，达到 V1 速度后抬轮\n";
    }
    double fuelConsumption() const override {
        return 250.0; // L/h
    }
};

// 水上飞机：起降方式又不同
class Seaplane : public Aircraft {
public:
    void takeOff() override {
        std::cout << "水上飞机：水面滑行，断阶后离水升空\n";
    }
    double fuelConsumption() const override {
        return 400.0; // L/h
    }
};

//关键点：纯虚函数 = "每个子类的起飞方式完全不同，基类没有合理的默认实现"。子类必须给出自己的版本，编译器强制检查。