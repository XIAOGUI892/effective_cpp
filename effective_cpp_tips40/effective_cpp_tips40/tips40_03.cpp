// 纯接口：定义"能做什么"
#include<string>
#include<iostream>
class IPerson {
public:
    virtual ~IPerson() = default;
    virtual std::string name() const = 0;
    virtual int age() const = 0;
};

// 工具实现类：提供"怎么存数据"（用条款 39 的 private 继承）
class Persistent {
public:
    void save() const {
        std::cout << "  持久化: " << serialize() << std::endl;
    } 
    void load(const std::string& data) {
        deserialize(data);
    }
private:
    virtual std::string serialize() const = 0;
    virtual void deserialize(const std::string&) = 0;
};

// ========== 具体类：接口 public 继承 + 持久化 private 继承 ==========
class Student : public IPerson,          //public：Student is-a IPerson
    private Persistent      //private：用 Persistent 实现存储
{
public:
    Student(std::string name, int age)
        : name_(std::move(name)), age_(age) {
    }

    // 实现 IPerson 接口
    std::string name() const override { return name_; }
    int         age()  const override { return age_; }

    // 提供序列化操作（委托给 Persistent）
    void saveToFile() const { Persistent::save(); }
    void loadFromFile(const std::string& d) { Persistent::load(d); }

private:
    std::string name_;
    int age_;

    // 实现 Persistent 的钩子函数
    std::string serialize() const override {
        return name_ + "," + std::to_string(age_);
    }
    void deserialize(const std::string& data) override {
        auto pos = data.find(',');
        name_ = data.substr(0, pos);
        age_ = std::stoi(data.substr(pos + 1));
    }
};

void test_student() {
    Student s("张三", 20);

    // 作为 IPerson 使用（public 继承）
    IPerson& p = s;
    std::cout << "姓名: " << p.name() << ", 年龄: " << p.age() << std::endl;

    // 使用持久化能力（来自 private 继承）
    s.saveToFile();                       // → 持久化: 张三,20
    s.loadFromFile("李四,25");
    s.saveToFile();                       // → 持久化: 李四,25

    // s.serialize();     //  编译错误！Persistent 被 private 继承，接口不暴露
    // Persistent* pp = &s; // 编译错误！不能向上转型到 private 基类
}