////有几种场景下，private 继承是合理甚至必需的：
//
////场景一：访问 protected 成员
//class Base {
//protected:
//    int protectedValue;
//    void protectedFunc();
//};
//
//class Derived_Composition {
//    // ❌ 普通复合无法访问 Base 的 protected 成员
//    Base b;
//    // void f() { b.protectedValue; }  // 编译错误
//};
//
//class Derived_Private : private Base {
//    void f() {
//        protectedValue = 42;      // ✅ 可以访问
//        protectedFunc();          // ✅ 可以调用
//    }
//};
//
//
//
////场景二：重写虚函数（这是最经典的场景）
//class Observer {
//public:
//    virtual void onDataChanged() = 0;
//    virtual void onError(int code) = 0;
//protected:
//    ~Observer() {}  // 条款7：多态基类应声明 virtual 析构函数，或 protected 非虚析构
//};
//
//class DataProcessor : private Observer {
//public:
//    void process() {
//        // ...
//        if (dataModified) onDataChanged();
//        if (hasError) onError(errorCode);
//    }
//
//private:
//    // 重写虚函数，但不暴露 Observer 接口给外部
//    virtual void onDataChanged() override {
//        // 内部处理逻辑
//    }
//
//    virtual void onError(int code) override {
//        // 内部错误处理
//    }
//
//    bool dataModified = false;
//    bool hasError = false;
//    int errorCode = 0;
//};
//
////设计思想： 这里 DataProcessor 需要 Observer 的回调机制，但这纯粹是实现细节。如果用 public 继承 + 复合，外部代码就能通过 Observer 指针操作
////DataProcessor，这违反了封装原则。
//
//
//
////场景三：空基类优化（EBO）
//class Empty {
//    // 没有任何非静态成员
//    typedef int typdef_type;     // 类型定义不占空间
//    static int static_data;      // 静态成员不占空间
//    void func();                 // 非虚函数不占空间
//};
//
//// 复合：因为 C++ 要求任何对象大小至少为 1，Empty 占 1 字节
//class Composite {
//    int x;
//    Empty e;   // 占 1 字节，加上对齐可能浪费更多
//};
//// sizeof(Composite) == 8 或更多（而不是 int 的 4）
//
//// private 继承：编译器可以做空基类优化，Empty 不占空间
//class Private_Inherit : private Empty {
//    int x;
//};
//// sizeof(Private_Inherit) == 4（Empty 不占空间）
//
////设计思想： C++ 标准允许对空基类做优化（EBO），这是唯一一个复合做不到而 private 继承能做到的优化场景。在 STL 中广泛使用，例如 std::unary_function 和
////std::binary_function 的子类都通过 private 继承实现。