#include <iostream>
#include <string>
 #include <vector>

 // ============================================================
 // 场景：一个模板基类，为不同类型提供通用的 print 方法
 // ============================================================
 template < typename Company>
 class MsgSender {
 public:
    void sendClear(const std::string & msg) {
        std::cout << "[MsgSender] Sending clear text to " << typeid(Company).name()
    << ": " << msg << std::endl;
        
    }
    
    void sendEncrypted(const std::string & msg) {
        std::cout << "[MsgSender] Sending ENCRYPTED to " << typeid(Company).name()
    << ": ***" << msg << "***" << std::endl;
        
    }
    
};

 /* ============================================================
  问题代码：派生类模板直接调用基类方法 → 编译失败
  ============================================================*/
  template <typename Company>
  class LoggingMsgSender : public MsgSender<Company> {
  public:
      void sendClearMsg(const std::string& msg) {
          // 编译错误！sendClear 找不到
          // 编译器不查找模板化基类，因为特化可能导致名字不存在
          this->sendClear(msg); 
		  // 解决方法1：使用 this-> 显式告诉编译器在基类中查找
          // this->sendClear(msg);
          // 
          // 解决方法2：使用 using 声明引入基类方法
          // using MsgSender<Company>::sendClear;
		  // sendClear(msg);
          //
		  // 解决方法3：直接使用基类限定符调用
		  // MsgSender<Company>::sendClear(msg);
		  //但有坏处：当sendClear是虚函数时，直接使用基类限定符调用会绕过虚函数机制，不会调用派生类的重写版本，可能导致意外行为！！！！ 
          //
      }
  };

 /* ============================================================
  为什么？因为模板可以被特化，而特化可能没有这个函数
  ============================================================*/
  int main() {
      LoggingMsgSender<std::string>ls;
	  ls.sendClearMsg("Hello, World!");
  }

  /*两阶段名称查找的核心设计思想

  两阶段分别做什么

  - 第一阶段(模板定义时)：检查所有非依赖名称(拼写不涉及模板参数的名称)。必须能独立找到，找不到立刻报错。
  - 第二阶段(实例化时)：检查依赖名称(拼写含模板参数的名称)。此时模板参数已知，编译器进入依赖基类查找。

  为什么不让所有检查都等到实例化时？

  1. 错误早发现，不甩锅给使用者

  模板作者笔误写成 sendClaer，如果等到用户实例化 LoggingMsgSender<SomeCompany> 时才报错，错误信息会指向用户的代码，而 bug 实际在模板定义里。第一阶段就报错，责任归属清楚。

  2. 特化不能悄悄改变语义

  这是最关键的理由。如果允许在第一阶段用全局函数、第二阶段用基类成员，那么：你传 CompanyA（基类有 sendClear）→ 调用基类版本；你传 CompanyB（特化删掉了 sendClear）→
  编译器静默回退到全局 ::sendClear。同样的调用写法，行为悄悄变了，不报警也不报错——这是最难排查的 bug。两阶段查找通过"非依赖名在第一阶段绑定"彻底杜绝了这种可能。

  3. 模板作者可为非依赖部分负责

  模板作者不需要知道用户会传什么类型，就能保证非依赖代码的正确性。this->
  的本质作用就是把名称变成依赖名称，明确告诉编译器"我去基类找，你第二阶段再查"——这是一个显式的语义承诺，而非编译器的技术限制。

  一句话总结

  两阶段查找把"名称解析"和"模板参数"解耦：不依赖参数的部分提前定死，只让真正依赖参数的部分推迟。这不是一刀切的妥协，而是让模板同时具备"早期错误检测"和"特化安全性"的刻意设计。*/