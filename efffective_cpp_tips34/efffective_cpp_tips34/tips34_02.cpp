
//当一个操作大多数子类做得一样，但少数子类需要特殊处理时使用。

#include <iostream>
#include <string>

class Airplane {
public:
    // 虚函数：提供接口 + 默认实现
    virtual void fly(const std::string& destination) {
        // 默认实现：大多数飞机通用的飞行方式
        std::cout << "飞往 " << destination
            << "，巡航高度 10000 米，速度 900 km/h\n";
    }
    virtual ~Airplane() = default;
};

// ModelA：使用默认飞行方式
class ModelA : public Airplane {
    // 不需要覆写 fly()，默认实现就很好
};

// ModelB：使用默认飞行方式
class ModelB : public Airplane {
    // 同样用默认实现
};

// ModelC：超音速战斗机——飞行方式完全不同，覆写
class ModelC : public Airplane {
public:
    void fly(const std::string& destination) override {
        std::cout << "飞往 " << destination
            << "，超音速巡航 2.5 马赫，高度 20000 米\n";
    }
};

int main() {
    ModelA a; a.fly("北京");   // 使用默认
    ModelB b; b.fly("上海");   // 使用默认
    ModelC c; c.fly("伦敦");   // 使用覆写版本
}

//这里有个陷阱：默认实现和接口绑在一起，子类可能静默地继承了不合适的默认实现。详见tips34_03A