#include <iostream>
#include <string>

class Airport {
public:
    // 纯虚函数：只声明接口，强制每个子类必须显式决定行为
    virtual bool canLand(const std::string& aircraftType) const = 0;
    virtual ~Airport() = default;

protected:
    // 普通函数：提供默认实现（供子类显式调用）
    bool defaultCanLand(const std::string& aircraftType) const {
        // 默认：大多数机场允许大多数机型降落
        return true;
    }
};

class RegionalAirport : public Airport {
public:
    bool canLand(const std::string& aircraftType) const override {
        // 普通机场：显式选择使用默认行为
        return defaultCanLand(aircraftType);
    }
};

class HighAltitudeAirport : public Airport {
public:
    bool canLand(const std::string& aircraftType) const override {
        // 高原机场：必须显式决定，（编译期）不可能"忘记"
        if (aircraftType == "A380" || aircraftType == "B747") {
            std::cout << "高原机场跑道过短，" << aircraftType << " 不能降落\n";
            return false;
        }
        return defaultCanLand(aircraftType);  // 其他机型可以
    }
};

//效果：任何子类试图编译却没有覆写 canLand()，直接编译失败。"忘记"变得不可能。