 // Effective C++ Item 44: 将与参数无关的代码抽离 templates
 // 使用书中案例——方阵求逆 SquareMatrix
 #include <iostream>

 // ============================================================
 // 反例(书中原始写法)：所有代码在模板里
 // ============================================================
 template < typename T, std::size_t n>
 class SquareMatrix {
 public:
         // invert 的算法只依赖矩阵尺寸 n，不依赖元素类型 T
            // 但 SquareMatrix<double,5> 和 SquareMatrix<float,5>
             // 会各自生成一份完全相同的 invert 机器码 → 代码膨胀
             void invert() {
                 std::cout << "invert: size=" << n << std::endl;
                 // ... 对矩阵数据做求逆运算 ...
           
    }
    
};

 // ============================================================
 // 正例(书中方案)：将 invert 抽到基类
 // ============================================================
 template <typename T>
 class SquareMatrixBase {
 protected:
    // 基类不关心类型，只关心数据指针和尺寸
        // 指针指向矩阵数据，尺寸作为函数参数传入
     T* pData;                         // 指向派生类中实际数据的指针
     std::size_t size;                 // 矩阵尺寸(运行时值，从模板参数 n 转化来)
    
    SquareMatrixBase(T * data, std::size_t n)
        : pData(data), size(n) {}
        // invert 放到基类：只依赖 size 参数，不依赖模板参数 n
        // 所有同类型 T 的矩阵（不论 n 是多少）共享这一个 invert
    void baseInvert() {
        std::cout << "baseInvert: using data at " << pData
    << ", size=" << size << std::endl;
        // ... 用 pData 和 size 做求逆运算 ...
            
    }
    
};

 template < typename T, std::size_t n>
 class SquareMatrixGood : private SquareMatrixBase<T> {
     T data[n * n];   // 数据存储在这里
 public:
    SquareMatrixGood() : SquareMatrixBase<T>(data, n) {}
    // invert 只是一个薄转发层——内联后零开销
    void invert() {
      this->baseInvert();   // Item 43: 依赖基类，必须用 this->
    }
    
};

 // ============================================================ // 效果对比
 // ============================================================
int main() {
      // 反例
    SquareMatrix<double, 5>  m1;
    SquareMatrix<double, 10> m2;
    m1.invert();   // SquareMatrix<double,5> :invert()  —— 一份代码
    m2.invert();   // SquareMatrix<double,10>::invert() —— 又一份代码
                // 两套 invert 算法完全相同，但机器码有两份
        
        // 正例
    SquareMatrixGood<double, 5>  m3;
    SquareMatrixGood<double, 10> m4;
    m3.invert();   // 都调用 SquareMatrixBase<double>::baseInvert()
    m4.invert();   // baseInvert 只有一份机器码！
                // size=5 和 size=10 的区别只是参数值不同
        
    std::cout << "\n=== 书中关键要点 ===" << std::endl;
    std::cout << "1. invert 算法只依赖 n(矩阵尺寸)，不依赖 T(元素类型)" << std::endl;
    std::cout << "2. 移到基类后，所有相同 T 的矩阵共享一个 invert" << std::endl;
    std::cout << "3. n 从编译期模板参数变成运行时函数参数(存储在 size 中)" << std::endl;
    std::cout << "4. 派生类中的 invert 只是薄转发，内联后零额外开销" << std::endl;
    std::cout << "5. 代价：基类需要存储指向派生类数据的指针，多一个指针的空间" << std::endl;
    
    return 0;
    
}