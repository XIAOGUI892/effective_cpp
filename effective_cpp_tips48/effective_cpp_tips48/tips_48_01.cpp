#include<iostream>
template<int n>
struct Factorial {
	static const int value = n * Factorial<n - 1>::value;
};

template<>
struct Factorial<1> {
	static const int value = 1;
};

int main() {
	
	std::cout << Factorial<5>::value<<std::endl;
}
/* main:
      sub     rsp, 8
      mov     esi, 120          ; ← 这就是 Factorial<5>::value 
      mov     edi, OFFSET FLAT:std::cout
      call    std::basic_ostream::operator<<(int)
      ; ... endl 调用 ...
      xor     eax, eax
      add     rsp, 8
      ret*/

/* 具体来说：

  1. 省 CPU 指令

  回想那个 mov esi, 120——如果不用 TMP，你在运行期算阶乘需要循环或递归：

  ; 运行期计算版本会多出这些：
      mov     eax, 1
      mov     ecx, 5
  .loop:
      imul    eax, ecx      ; 乘法指令
      dec     ecx            ; 计数器减一
      jne     .loop          ; 循环跳转

  每次程序运行都要走一遍这条路径。TMP 版呢？一条 mov esi, 120，没了。省了乘法、分支、跳转。

  2. 省内存访问

  运行期计算通常需要栈帧、局部变量。TMP 的 static const 常量直接嵌入指令中的立即数，不需要栈空间，不需要读内存。

  3. 编译器可以在此基础上继续优化

  一旦 Factorial<5>::value 被折叠成 120，后续优化器就能利用这个常量。比如：

  int arr[Factorial<5>::value];   // → int arr[120];  数组大小已知，栈分配一次到位
  int x = Factorial<5>::value * 2; // → int x = 240;   常量传播
  if (Factorial<5>::value > 100)  // → if (true)       死代码消除，else 分支直接删掉

  4. 量变引起质变

  一个 5! 省不了几纳秒，但实际应用中 TMP 用在：

  - STL 迭代器：vector<int>::iterator 的 += n 在编译期就选定 O(1) 的实现，而不是运行期 if/else 判断是哪类迭代器
  - 矩阵运算库（如 Eigen）：编译期已知矩阵维度，循环完全展开，寄存器分配最优，没有循环开销
  - static_assert：直接把类型错误扼杀在编译期，连运行的机会都没有

  一句话总结： 编译期花 0.1
  秒多算一点，换来运行期每次调用都省一条分支、一次乘法、一次内存访问。一个函数被调用一百万次，就省一百万次。代价是一次性的编译时间，收益是永久的运行效率。*/