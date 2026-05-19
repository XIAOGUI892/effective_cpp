#include<iostream>
using namespace std;
class Shape {
public:
    virtual void draw(int color = 0xFF0000) {   // 缺省: 红色
        cout << "Shape::draw, color=" << hex << color << endl;
    }
};

class Circle : public Shape {
public:
    void draw(int color = 0x00FF00) override {  // 缺省: 绿色 — 不一样！
        cout << "Circle::draw, color=" << hex << color << endl;
    }
};

int main() {
    Circle c;
    Shape* p = &c;       // 基类指针，指向子类对象

    p->draw();            // 动态绑定 → Circle::draw
    // 缺省值 → 0xFF0000（Shape 的，不是 Circle 的！）
    // 输出: "Circle::draw, color=ff0000"
	//参数 color 的缺省值是根据指针类型（Shape*）来决定的，而不是根据对象类型（Circle）。因此，虽然调用了 Circle::draw，但使用了 Shape::draw 中定义的缺省参数值。
    /*
  1. 看到 draw() 在 Shape 中声明为 draw(int color = 0xFF0000)
  2. p 的静态类型是 Shape*，缺省值是 0xFF0000
  3. 它生成的机器码，效果等价于： p->draw(0xFF0000);

  生成的汇编大致是：

  mov  edi, 0xFF0000    ; 把缺省值 0xFF0000 写入 edi 寄存器（第一个参数）
  mov  rax, [p]         ; 取 vptr
  call [rax + N]        ; 间接调用 Circle::draw*/
}
