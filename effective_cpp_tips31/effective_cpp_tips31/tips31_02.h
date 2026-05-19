#include<memory>
class Date;
class Address;
class PersonImpl;
class Person {
private:
	std::shared_ptr<PersonImpl>pImpl;
};
//
