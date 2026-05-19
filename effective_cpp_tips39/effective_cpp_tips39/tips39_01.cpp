//一般情况下：private继承与复合都表示根据某物实现出
//但我们优先考虑复合

 // === 方案A：复合（首选） ===
class Timer {
public:
    virtual void onTick() const;
};

class Widget_A {
private:
    // 用一个嵌套类来重写 Timer 的虚函数
    class WidgetTimer : public Timer {
    public:
        virtual void onTick() const override {
            // 实际的 tick 逻辑
        }
    };
    WidgetTimer timer;  // 复合
};

// === 方案B：private 继承 ===
class Widget_B : private Timer {
private:
    virtual void onTick() const override {
        // 实际的 tick 逻辑
    }
};

//为什么复合更优？ 有三个原因：
//  
//原因一：防止派生类进一步重写
//
//// 用复合，Widget_A 的派生类无法重写 onTick()
//class SpecialWidget_A : public Widget_A {
//    // ❌ 无法重写 onTick() —— WidgetTimer 被封装在 Widget_A 内部，对子类不可见
//};
//
//// 用 private 继承，Widget_B 的派生类可以重写 onTick()
//class SpecialWidget_B : public Widget_B {
//private:
//    virtual void onTick() const override {  // ✅ 可以重写！
//        // 这可能不是 Widget_B 作者想要的行为
//    }
//};
//
//设计思想： private 继承虽然阻止了外部访问，但不能阻止派生类重写虚函数。如果你的设计中不希望被子类修改某些行为，复合可以提供更强的封装。
// 
//=====================================================================================
//原因二：降低编译依赖
//
//// Widget_A.h（复合）
//#include "Timer.h"   // 必须包含，因为 WidgetTimer 需要完整定义
//class Widget_A { /* ... */ };
//
////// Widget_B.h（private 继承）
//#include "Timer.h"   // 同样必须包含
//class Widget_B : private Timer { /* ... */ };
//
//在上面的简单例子中两者一样。但如果使用 pimpl 惯用法与复合结合：
//
//// Widget.h —— 不需要包含 Timer.h
//class Widget {
//private:
//    class Impl;           // 前向声明即可
//    std::unique_ptr<Impl> pImpl;
//};
//
//// Widget.cpp —— Timer 的依赖隐藏在实现文件中
//class Widget::Impl : public Timer {
//    virtual void onTick() const override { /* ... */ }
//};
//
//设计思想： 复合 + pimpl 可以将实现依赖完全隐藏在.cpp 文件中，大幅减少编译依赖。private 继承做不到这一点。
// 
// 
// 
//=================================================================================================
//原因三：更清晰的语义
//
//class Set {
//private:
//    std::vector<int> data;  // 清晰地表达：Set 有一个 vector 作为内部存储
//    // 而不是 Set 就是一个 vector
//};
//
//复合使 "has-a" 语义一目了然。
