#include<iostream>
using namespace std;
class Shape {
public:
    void draw(int color = 0xFF0000) {   // 缺省: 红色
        doDraw(color);
    }
private:
    virtual void doDraw(int color) {
        cout << "Shape::doDraw, color=" << hex << color << endl;
    }

};

class Circle : public Shape {
private:
    void doDraw(int color) override {  // 没有缺省参数了
        cout << "Circle::doDraw, color=" << hex << color << endl;
    }
};
int main() {
    Circle c;
    Shape* p = &c;       // 基类指针，指向子类对象
    p->draw();            // 动态绑定 → Circle::doDraw
	// 输出: "Circle::doDraw, color=ff0000"
}