//#include<iostream>
//#include<memory>
//class Point {
//private:
//	int X;
//	int Y;
//public:
//	Point(int x,int y):X(x),Y(y){}
//	void  setX(int x) { X = x; };
//	void  setY(int y) { Y = y; };
//	int getX()const {
//		return X;
//	}
//	int getY()const {
//		return Y;
//	}
//};
//
//struct RectData {
//	Point ulhc_;
//	Point lrhc_;
//public:
//	RectData(Point ulhc, Point lrhc):ulhc_(ulhc),lrhc_(lrhc){}
//};
//
//class Rectangle {
//private:
//	std::shared_ptr<RectData>pData;
//public:
//	Point& upperLeft()const {
//		return pData->lrhc_;
//	}
//	Point& lowerRight()const {
//		return pData->ulhc_;
//	}
//	Rectangle(Point ulhc,Point lrhc):pData(std::make_shared<RectData>(ulhc,lrhc)){}
//};
//
//int main() {
//	Rectangle rec(Point(1,2),Point(3,4));
//	Point &point = rec.upperLeft();
//	std::cout << rec.upperLeft().getX() << std::endl;
//	point.setX(6);
//	std::cout << rec.upperLeft().getX() << std::endl;
//	//问题出现：res中的值被修改
//	//原因：upperLeft()返回point引用，导致可以通过外部修改类内的private成员
//	//破坏了类的封装性
//	// ===================================================
//	//解决方法：返回值加const
//}