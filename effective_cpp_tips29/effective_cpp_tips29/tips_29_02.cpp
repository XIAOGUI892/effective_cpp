#include<iostream>
#include<memory>
class Point {
private:
	int X;
	int Y;
public:
	Point(int x, int y) :X(x), Y(y) {}
	void  setX(int x) { X = x; };
	void  setY(int y) { Y = y; };
	int getX()const {
		return X;
	}
	int getY()const {
		return Y;
	}
};

struct RectData {
	Point ulhc_;
	Point lrhc_;
public:
	RectData(Point ulhc, Point lrhc) :ulhc_(ulhc), lrhc_(lrhc) {}
};

class Rectangle {
private:
	std::shared_ptr<RectData>pData;
public:
	const Point& upperLeft()const {
		return pData->lrhc_;
	}
	const Point& lowerRight()const {
		return pData->ulhc_;
	}
	Rectangle(Point ulhc, Point lrhc) :pData(std::make_shared<RectData>(ulhc, lrhc)) {}
};

int main() {
	Rectangle rec(Point(1, 2), Point(3, 4));
	const Point& point1 = rec.upperLeft();
	std::cout << rec.upperLeft().getX() << std::endl;
	//point.setX(6);直接报错！！！
	std::cout << rec.upperLeft().getX() << std::endl;

	//======================
	const Point& point2 = Rectangle(Point(1, 2), Point(3, 4)).upperLeft();
	std::cout << point2.getX();
	//问题出现：Point变为悬空引用
	//原因：Rectangle(Point(1, 2), Point(3, 4))是一个临时对象，这条语句结束后该对象就销毁，而由于upperLeft返回的是引用，对象销毁，则成员变量也没有定义，point2成为悬空引用
	//解决办法：以后尽量避免函数返回引用，指针，迭代器这些直接返回对象内部结构的变量，同时可以帮助const函数更加像一个const函数
	
}