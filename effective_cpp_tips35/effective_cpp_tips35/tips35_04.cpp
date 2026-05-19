#include <iostream>
#include <memory>
class HealthCalcAlgo {
public:
    virtual int calc(const class GameCharacter& gc) const = 0;
    virtual ~HealthCalcAlgo() = default;
};
class GameCharacter {
public:
    GameCharacter(std::unique_ptr<HealthCalcAlgo>ptr):up(std::move(ptr)){}
    int getLevel() const{
        return level_;
    }
    int healthValue() {
        return up->calc(*this);
    }
    void resetAlgo(std::unique_ptr<HealthCalcAlgo>ptr) {
        up = std::move(ptr);
    }
private:
    std::unique_ptr<HealthCalcAlgo>up;
    int level_;
};
// 策略基类：血量计算算法族


// 具体策略1：默认算法
class DefaultHealthCalc : public HealthCalcAlgo {
public:
    int calc(const GameCharacter& gc) const override {
        return 50 + gc.getLevel() * 10;
    }
};

// 具体策略2：战士算法
class WarriorHealthCalc : public HealthCalcAlgo {
public:
    int calc(const GameCharacter& gc) const override {
        return 80 + gc.getLevel() * 12;
    }
};

// 具体策略3：受伤状态算法
class InjuredHealthCalc : public HealthCalcAlgo {
public:
    int calc(const GameCharacter& gc) const override {
        return 20 + gc.getLevel() * 5;  // 受伤时血量降低
    }
};
int main() {
    GameCharacter gc1(std::make_unique<DefaultHealthCalc>());
}