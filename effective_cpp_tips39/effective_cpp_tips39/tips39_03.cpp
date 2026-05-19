#include <iostream>
#include<string>
class Timer {
public:
    // 关键：这是基类自己的函数，内部通过 this 调用虚函数
    void start(const std::string& taskName) {
        std::cout << "[Timer::start] 定时器启动，准备回调..." << std::endl;

        // 这里的 this 静态类型是 Timer*
        // 但运行时动态类型是 Widget（如果 this 是 Widget 的 Timer 子对象）
        // 虚函数分派 → 会调用 Widget::onTick()！
        this->onTick(taskName);
    }

private:
    // 基类的虚函数 —— 可以是纯虚函数，也可以有默认实现
    virtual void onTick(const std::string& taskName) {
        std::cout << "[Timer::onTick] 基类默认实现: " << taskName << std::endl;
    }
protected:
    ~Timer() {}  // protected 析构：只允许派生类持有
};

class Widget : private Timer {
public:
    void scheduleTask(const std::string& name) {
        std::cout << "[Widget::scheduleTask] 对外接口被调用" << std::endl;
        start(name);  //  可以调用基类的 public 函数
    }

private:
    // 重写基类虚函数 —— 外部不能调用，但基类内部能调到
    void onTick(const std::string& taskName) override {
        std::cout << "[Widget::onTick] === 派生类重写的版本被触发！===" << std::endl;
        std::cout << "[Widget::onTick] 正在处理任务: " << taskName << std::endl;
    }
};

int main() {
    Widget w;
    w.scheduleTask("发送邮件");

    // w.onTick("xxx");   //  编译错误：onTick 是 private
    // w.start("xxx");    //  编译错误：start 被 private 继承变成 private
    // Timer* p = &w;     //  编译错误：不能向上转型

    return 0;
}