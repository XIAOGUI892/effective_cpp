#include <vector>
#include<iostream>
class SpeedDataCollection {
public:
    void addValue(int speed) {
        sum_ += speed;
        count_++;
        average_ = static_cast<double>(sum_) / count_;
    }

    double averageSoFar() const {
        return average_;  // 直接返回预计算的值
    }

private:
    int sum_ = 0;        // 累计总量
    int count_ = 0;      // 数据点数
    double average_ = 0; // 当前平均值
};

class SpeedDataCollection {
public:
    void addValue(int speed) {
        speeds_.push_back(speed);
    }

    double averageSoFar() const {
        if (speeds_.empty()) return 0.0;
        int sum = 0;
        for (int speed : speeds_) {
            sum += speed;
        }
        return static_cast<double>(sum) / speeds_.size();
    }

private:
    std::vector<int> speeds_;  // 存储所有速度数据
};

void processSpeedData(SpeedDataCollection& collection) {
    collection.addValue(60);
    collection.addValue(80);
    double avg = collection.averageSoFar();  // 只依赖接口，不依赖实现
    std::cout << "Average speed: " << avg << std::endl;
}
int main() {
    //将成员变量设置为private，当类源码改变时，客户端代码无需改变，提高了代码的封装性
   
}