//class GameCharacter {
//public:
//	using healthCal = int (*)(const GameCharacter&);
//	int healthValue() {
//		return healthFunc_(*this);
//	};
//	GameCharacter(healthCal healthFunc):healthFunc_(healthFunc){}
//	void resetHealthFunc(healthCal newHealthFunc){
//		healthFunc_ = newHealthFunc;
//	}
//	int getLevel()const {
//		return level;
//	}
//private:
//	healthCal healthFunc_;
//	int level;
//};
//// 战士的计算方式——另一个自由函数
//int warriorHealthCalc(const GameCharacter& gc) {
//	return 80 + gc.getLevel() * 10;  // 80 + 5*10 = 130
//}
//
//// 法师的计算方式
//int mageHealthCalc(const GameCharacter& gc) {
//	return 30 + gc.getLevel() * 5;   // 30 + 5*5 = 55
//}
//int main() {
//	GameCharacter warrior(warriorHealthCalc);
//	GameCharacter mage(mageHealthCalc);
//	/*优缺点：
//  -  同一个类型可以有不同计算方式（不靠子类，靠构造函数参数）
//  -  可以在运行时换算法：obj.setHealthCalc(newFunc)
//  -  算法函数是自由函数，和类解耦
//  -  函数只能通过 public 接口访问对象内部，如果需要的都是私有数据就麻烦了*/
//}