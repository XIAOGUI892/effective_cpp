//#include<iostream>
//#include<functional>
//class GameCharacter {
//public:
//	using healthCalFunc = std::function<int(const GameCharacter& character)>;
//	GameCharacter(healthCalFunc healthFunc):healthFunc_(healthFunc){}
//	void resetHealthFunc(healthCalFunc newHealthFunc) {
//		healthFunc_ = newHealthFunc;
//	}
//	int healthValue() {
//		return healthFunc_(*this);
//	}
//	int getLevel()const {
//		return level_;
//	}
//private:
//	healthCalFunc healthFunc_;
//	int level_;
//};
//int warriorHealthFunc(const GameCharacter &gc) {
//	return gc.getLevel() * 10;
//}
//struct Warrior {
//	int operator()(const GameCharacter& gc) {
//		return gc.getLevel() * 10;
//	}
//};
//int main() {
//	GameCharacter gc1(warriorHealthFunc);
//	GameCharacter gc2(Warrior());
//	GameCharacter gc3([](const GameCharacter& gc) {return gc.getLevel() * 10; });
//	/*优缺点：
//  -  接受任何可调用对象——函数指针、lambda、仿函数、std::bind 结果
//  -  运行时切换
//  -  std::function 内部有类型擦除开销，些许间接调用成本*/
//}