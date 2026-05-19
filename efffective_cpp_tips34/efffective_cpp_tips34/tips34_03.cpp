//陷阱案例：子类"忘记"覆写导致的 Bug

#include <iostream>
#include <string>

//  有问题的设计
class Airport {
public:
    virtual bool canLand(const std::string& aircraftType) {
        // 默认：大多数机场允许大多数机型降落
        return true;
    }
    virtual ~Airport() = default;
};

// 普通机场：用默认实现，大多数飞机都能降落
class RegionalAirport : public Airport {};

// 高原机场：跑道短，只能降落特定机型
// 程序员忘记覆写 canLand()！
class HighAltitudeAirport : public Airport {
    // 忘记覆写 canLand() — 继承了默认的 return true
    // 这会导致：巨型客机也被告知"可以降落"，造成事故！
};

// 正确做法：把接口和默认实现分离（见下一个例子)