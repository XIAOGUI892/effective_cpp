#pragma once
#include<string>
class Date {
public:
    // 构造函数
    Date();
    Date(int year, int month, int day);

    // 访问器
    int getYear() const;
    int getMonth() const;
    int getDay() const;
    std::string toString() const;

    // 修改器
    void addDays(int days);
    void addMonths(int months);
    void addYears(int years);

private:
    // 实现细节
    int year_;
    int month_;
    int day_;

    // 辅助函数
    bool isLeapYear(int year) const;
    int daysInMonth(int year, int month) const;
};
