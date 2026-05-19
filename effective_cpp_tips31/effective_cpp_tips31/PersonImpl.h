#pragma once
// PersonImpl.h - 实现类的定义（只在 .cpp 中包含）
#include <string>
#include "Date.h"
#include "Address.h"
class PersonImpl {
public:
    PersonImpl(const std::string& name, const Data& birthday, const Address& addr);
    std::string name() const;
    std::string birthDate() const;
    std::string address() const;

private:
    std::string theName;
    Data theBirthDate;
    Address theAddress;
};
//好处：当Data或Address变化时，只需要PersonImpl.cpp文件重新编译，十分方便