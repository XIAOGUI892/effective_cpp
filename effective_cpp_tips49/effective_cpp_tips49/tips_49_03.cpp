/* 第二个矛盾：每个类都要写一遍这套代码

  上面的代码有一个明显的问题——如果 DatabaseConnection、NetworkBuffer、LargeArray 都需要类专属 new-handler，每个类都要把这套 set_new_handler + operator new
  原封不动地抄一遍。唯一不同的只是 currentHandler 的类型（它们各有一个各自的 static 成员）。*/

//解决方案：Mixin 基类模板
//
//把上面那套逻辑抽到一个基类模板里：

#include <new>
#include<iostream>
template<typename T>    // T 只是用来让不同继承获得不同的 static 成员
class NewHandlerSupport {
public:
    static std::new_handler set_new_handler(std::new_handler h) noexcept {
        std::swap(currentHandler, h);
        return h;
    }

    static void* operator new(std::size_t size) {
        std::new_handler globalHandler = std::set_new_handler(currentHandler);

        void* memory;
        try {
            memory = ::operator new(size);
        }
        catch (...) {
            std::set_new_handler(globalHandler);
            throw;
        }
        std::set_new_handler(globalHandler);
        return memory;
    }

private:
    static std::new_handler currentHandler;
};

template<typename T>
std::new_handler NewHandlerSupport<T>::currentHandler = nullptr;

//现在任何类想拥有自己的 new - handler，只需继承这个 mixin：

class Image : public NewHandlerSupport<Image> {
    // 不需要再写 set_new_handler 和 operator new
    // 只需专心写 Image 的业务逻辑
    char s[1000];
};

class DatabaseConnection : public NewHandlerSupport<DatabaseConnection> {
    // 同上
};
/*为什么模板参数 T 要写成 NewHandlerSupport<Image>？

  因为 static 成员在模板中每套实例化类型各有一份独立的副本。如果不用模板：

  class Image         : public NewHandlerSupport {};  // 同一个基类
  class DatabaseConn  : public NewHandlerSupport {};  // 同上

  两个类共享同一个 currentHandler，这就失去了"类专属"的意义。模具参数 T 把 NewHandlerSupport<Image>::currentHandler 和 NewHandlerSupport<DatabaseConnection>::currentHandler
  变成了两个不同的 static 变量。

  这种技巧叫 CRTP (Curiously Recurring Template Pattern)——派生类把自己作为模板参数传给基类，利用模板的"不同实例化得到不同副本"这个特性。*/

void handler() {
    std::cerr << "error" << std::endl;
}
int main() {
    Image::set_new_handler(handler);
    Image* ptr = (Image *)Image::operator new(100000000000000000);
    return 0;
}