//#include<iostream>
//class GameCharacter {
//public:
//	int healthCal() {
//		std::cout << "血量计算开始!" << std::endl;//前置工作
//		int res = doHealthValue();
//		if (res < 0)res = 0;//后置工作
//		if (res > 100)res = 100;//后置工作
//		std::cout << "血量为： " << res << std::endl;
//		return res;
//	}
//	virtual ~GameCharacter() = default;
//private:
//	virtual int doHealthValue() const = 0;
//};
//class Warrior :public GameCharacter{
//private:
//	virtual int doHealthValue()const override{
//		return 90;
//	}
//};
//
//class Mage : public GameCharacter {
//private:
//	int doHealthValue() const override {
//		return 40;   // 法师血量40
//	}
//};
///* 基类对流程有绝对控制权（前置/后置、加锁等绝对同步）
//  -  子类只能定制"被允许定制"的部分
//  -  虚函数的本质还在，有 vtbl 和间接调用开销*/
//int main() {
//	Warrior w;
//	w.healthCal();
//	Mage m;
//	m.healthCal();
//}