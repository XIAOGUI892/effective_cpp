constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}
/*constexpr 是 C++11 引入的关键字，告诉编译器"这玩意可以在编译期算出来"。

  它与 TMP 做的是同一件事——编译期计算——但写法是普通函数，而不是递归模板*/

constexpr int square(int x) {
    return x * x;                    // OK：就一行 return
}

constexpr int abs(int x) {
    return x < 0 ? -x : x;          // OK：三元也是单条 return
}

constexpr int factorial(int n) {
    // ❌ 有多条语句，C++11 不行
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

//C++14（大幅放宽）： 可以有分支、循环、多条语句

constexpr int factorial(int n) {     // C++14 OK
    int result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

//C++17 / 20（进一步放宽）： 支持 try、throw（编译期抛异常算编译错误）、constexpr 虚函数、constexpr new / delete、constexpr vector / string 等

/*硬性限制（始终不能加）

- 不能有未定义行为（编译期会直接报错）
- 不能有 goto
- 不能有 asm 内联汇编
- 不能调用非 constexpr 函数（因为运行期才能执行的内容编译期无法动用）

快速判断法

问自己：这个函数体里的所有操作，编译器能不能在你没运行程序的情况下直接算出来？ 能，就可以加。不能（比如要读文件、联网、分配内存后不释放、调用
rand()），就不行。*/