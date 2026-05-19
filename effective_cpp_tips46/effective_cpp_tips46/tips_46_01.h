#include <iostream>
#include <type_traits>
#include <typeinfo>
// ============================================================================
// 辅助：打印类型名（编译器无关）
// ============================================================================
template <typename T>
const char* type_name() {
    static const char* s = abi::__cxa_demangle(typeid(T).name(), 0, 0, nullptr);
    return s;
}

// ============================================================================
// 阶段一：天真的写法 —— 模板外的 operator*
// ============================================================================
namespace stage1 {

    template <typename T>
    class Rational {
    public:
        // 允许从 int 隐式构造
        Rational(const T& numerator = 0, const T& denominator = 1)
            : n_(numerator), d_(denominator) {
        }

        const T& numerator()   const { return n_; }
        const T& denominator() const { return d_; }

    private:
        T n_, d_;
    };

    // 函数模板 —— 非成员 operator*
    template <typename T>
    const Rational<T> operator*(const Rational<T>& lhs,
        const Rational<T>& rhs) {
        return Rational<T>(lhs.numerator() * rhs.numerator(),
            lhs.denominator() * rhs.denominator());
    }

    void demo() {
        std::cout << "=== 阶段一：模板外的 operator* ===\n";

        Rational<int> oneHalf(1, 2);
        Rational<int> result1 = oneHalf * Rational<int>(2, 1); // OK

        // Rational<int> result2 = oneHalf * 2;   // ❌ 编译错误！
        //
        // 为什么？编译器尝试推导 T:
        //   参数1: Rational<int> → T = int
        //   参数2: int           → 无法把 int 匹配成 Rational<T>
        // 函数模板的参数推导从不考虑隐式类型转换。
        // 于是推导失败。

        std::cout << "  oneHalf * Rational<int>(2) → OK\n";
        std::cout << "  oneHalf * 2                → 编译错误\n\n";
    }

} // namespace stage1


// ============================================================================
// 阶段二：在类内部声明 friend 函数 —— 但仅在类内声明
// ============================================================================
namespace stage2 {

    template <typename T>
    class Rational {
    public:
        Rational(const T& numerator = 0, const T& denominator = 1)
            : n_(numerator), d_(denominator) {
        }

        const T& numerator()   const { return n_; }
        const T& denominator() const { return d_; }

        // 在类内声明 friend
        friend const Rational<T> operator*(const Rational<T>& lhs,
            const Rational<T>& rhs);

    private:
        T n_, d_;
    };

    // 在类外定义
    template <typename T>
    const Rational<T> operator*(const Rational<T>& lhs,
        const Rational<T>& rhs) {
        return Rational<T>(lhs.numerator() * rhs.numerator(),
            lhs.denominator() * rhs.denominator());
    }

    void demo() {
        std::cout << "=== 阶段二：类内声明 friend，类外定义 ===\n";

        Rational<int> oneHalf(1, 2);

        // Rational<int> result = oneHalf * 2;   // ❌ 仍然是编译错误！
        //
        // 为什么？虽然类内声明了一个 friend 函数，但只是声明而已。
        // 类外定义仍然是一个函数模板。
        // 编译器在 oneHalf * 2 的调用点，需要找到匹配的 operator*。
        // 它找到的是函数模板 operator*<T>，模板推导再次失败。
        // friend 声明本身创建的是非模板函数签名，但那个函数没有被定义——
        // 模板定义和 friend 声明之间没有连接。

        std::cout << "  oneHalf * 2 → 仍然是编译错误\n\n";
    }

} // namespace stage2


// ============================================================================
// 阶段三：正确做法 —— 在类内部同时声明与定义 friend 函数
// ============================================================================
namespace stage3 {

    template <typename T>
    class Rational {
    public:
        Rational(const T& numerator = 0, const T& denominator = 1)
            : n_(numerator), d_(denominator) {
        }

        const T& numerator()   const { return n_; } 
        const T& denominator() const { return d_; }

        // ✅ 关键：在类内部直接定义 friend 函数体
        // 这为每个 Rational<T> 实例化生成一个独立的非模板函数。
        // 例如 Rational<int> 实例化后，编译器内部生成：
        //
        //   const Rational<int> operator*(const Rational<int>& lhs,
        //                                 const Rational<int>& rhs) {
        //       return Rational<int>(lhs.n_ * rhs.n_, lhs.d_ * rhs.d_);
        //   }
        //
        // 这是一个真正的函数（不是模板），因此：
        //   oneHalf * 2  中，编译器看到 operator*(Rational<int>, Rational<int>)
        //   发现参数 2 可以隐式转换为 Rational<int>（via 单参构造函数）
        //   → 转换合法，调用成功！

        friend const Rational<T> operator*(const Rational<T>& lhs,
            const Rational<T>& rhs) {
            // 直接访问私有成员（因为它是 friend）
            return  
        }

    private:
        T n_, d_;
    };

    void demo() {
        std::cout << "=== 阶段三：类内定义 friend 函数（正确做法）===\n";

        Rational<int> oneHalf(1, 2);

        auto r1 = oneHalf * Rational<int>(2, 1);
        std::cout << "  oneHalf * Rational<int>(2,1) = "
            << r1.numerator() << "/" << r1.denominator() << "\n";

        auto r2 = oneHalf * 2;   // ✅ 编译通过！
        std::cout << "  oneHalf * 2                  = "
            << r2.numerator() << "/" << r2.denominator() << "\n";

        auto r3 = 3 * oneHalf;   // ✅ 也通过！
        // 解释：3 → 隐式构造 Rational<int>(3) → 调用 operator*(Rational<int>, Rational<int>)
        std::cout << "  3 * oneHalf                  = "
            << r3.numerator() << "/" << r3.denominator() << "\n";

        auto r4 = oneHalf * 2 * 3; // ✅ 连续混合运算
        std::cout << "  oneHalf * 2 * 3              = "
            << r4.numerator() << "/" << r4.denominator() << "\n\n";
    }

} // namespace stage3

namespace stage4 {
    template<typename T>
    class Rational {
    public:
        const T& numerator() const{ return n_; }
        const T& denominator()const { return d_; }
        friend const T operator*(const Rational& lhs, const Rational& rhs) {
            return doMutiply(lhs, rhs);
        }
    private:
        T n_, d_;
    private:
        const T doMutiply(const Rational& lhs, const Rational& rhs);
    };

    template< typename T>
    const T doMutiply(const Rational<T>& lhs, const Rational<T>& rhs) {
        return Rational<T>(lhs.n_ * rhs.n_, lhs.d_ * rhs.d_);
    }
}
//inline函数会导致代码膨胀，当然，在这个案例中friend inline函数的代码只有一行，影响不大，但对于某些代码量比较大的inline函数，在inline函数中调用另外一个函数
//能降低这种膨胀
//当然：这种做法在现代编译器中没有降低代码膨胀的价值，现代编译器都很智能，会做隐形优化