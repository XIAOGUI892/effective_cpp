//两个例外（不加 typename 的地方）

template<typename T>
class Base {
public:
	class NestedType {};  // 嵌套类型
};
template<typename T>
class Derived {
public:
	Derived();
};

//1. 基类列表中：编译器本来就知道只能是类型
template<typename T>
class Derived : public Base<T>::NestedType {  // 不用加 typename
};

//2. 成员初始化列表中：同理
template<typename T>
Derived<T>::Derived() : Base<T>::NestedType(x) {  // 不用加 typename
}