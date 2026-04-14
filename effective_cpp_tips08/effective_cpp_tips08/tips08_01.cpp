#include<exception>
class Socket{
    public:
    Socket() {
        // 初始化套接字...
    }
    ~Socket() {
        // 关闭套接字...
    }
    void close() {
        // 关闭套接字...
	}
};
class NetworkConnection {
    public:
    NetworkConnection() {
        // 连接网络...
    }
    ~NetworkConnection() {                                                                 /**/
        // 断开网络连接...
		socket.close();  // 可能抛出异常

	}
private:
    Socket socket;
};
class ProcessingException : public std::exception {
    public:
    const char* what() const noexcept override {
        return "Processing error occurred.";
    }
};
void processData() {
    try {
        NetworkConnection conn;  // 步骤 1：创建 conn 对象
        // 处理数据...
        throw ProcessingException();  // 步骤 2：抛出第一个异常
    }
    catch (...) {  // 步骤 5：尝试捕获异常
        // 清理工作...
    }
}
int main() {
    try {
		processData();//为什么说这段代码存在问题？因为在 processData 函数中，
        //虽然 NetworkConnection 对象 conn 会在异常发生时自动调用析构函数进行清理，
		//但如果该析构函数调用过程中抛出未处理的异常，就会导致程序崩溃，因为在 C++ 中，如果在处理另一个异常时又抛出一个异常，程序会调用 std::terminate() 终止程序。
	   //因此，正确的做法是在析构函数中捕获任何可能抛出的异常，确保资源能够正确释放，而不会导致程序崩溃,或者保证析构函数不会抛出异常。详见tip08_02.cpp。
    }
    catch (const std::exception& e) {
        // 处理异常...
	}
}