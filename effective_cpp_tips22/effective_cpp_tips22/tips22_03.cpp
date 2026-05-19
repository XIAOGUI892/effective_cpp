#include <vector>
#include <iostream>
class SpeedDataCollection {
protected:
    std::vector<int> speeds_;  // protected 成员变量

public:
    void addValue(int speed) {
        speeds_.push_back(speed);
    }

    double averageSoFar() const {
        if (speeds_.empty()) return 0.0;
        int sum = 0;
        for (int s : speeds_) sum += s;
        return static_cast<double>(sum) / speeds_.size();
    }
};
class EnhancedSpeedData : public SpeedDataCollection {
public:
    void processAllSpeeds() {
        // 直接访问 protected 成员
        for (int speed : speeds_) {  //  可以访问
            std::cout << speed << std::endl;
        }

        // 直接修改 protected 成员
        speeds_.push_back(200);  //  可以修改
    }

    void clearSpeeds() {
        speeds_.clear();  //  可以清空
    }
};
//将成员变量设置为protected，Derived class 可以随意更改成员变量，封装性差