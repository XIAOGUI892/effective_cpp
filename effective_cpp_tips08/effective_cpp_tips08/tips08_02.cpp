#include<exception>
class Socket {
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
                                                                                           
    NetworkConnection() {
        // 连接网络...
    }
    ~NetworkConnection() {
        try {
            socket.close();  // 可能抛出网络异常    
        }
        catch (const NetworkException& e) {
            // 记录错误，但不重新抛出 
            Logger::error("Failed to close socket: " + e.what());
        }
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


    //也可以将资源释放的权力交给客户端，提供一个显式的 close() 方法来释放资源，并在析构函数中确保不抛出异常。
	// 例如：
    // 
class NetworkConnection {                                                
public:
    void close(){
        try{
            socket.close();
            closed = true;  // 可能抛出网络异常    
        }
        catch (const NetworkException& e) {
            // 记录错误，但不重新抛出 
            Logger::error("Failed to close socket: " + e.what());
		}
    }
    NetworkConnection() {
        // 连接网络...
    }
    ~NetworkConnection() {
        if (!closed) {
            try {
                socket.close();  // 可能抛出网络异常    
            }
            catch (const NetworkException& e) {
                // 记录错误，但不重新抛出 
                Logger::error("Failed to close socket: " + e.what());
            }
        }
    }
private:
    Socket socket;
    bool closed;
};