/*═══════════════════════════════════════════════════════════════════════════════
 * 条款 50 理由二：调试和错误检测 —— 哨兵字节
 *═══════════════════════════════════════════════════════════════════════════════
 *
 * 常见的内存 bug：
 *   1. 向前越界写（underrun）：比如写 arr[-1]
 *   2. 向后越界写（overrun）： 比如写 arr[size]（最常见）
 *   3. 重复释放（double free）
 *   4. 释放后使用（use-after-free）
 *
 * 哨兵机制的思想：
 *   在用户数据块的前后各放一个魔数（独家字节值），释放时检查。
 *   如果魔数被篡改 → 有人踩了不该踩的地址 → 报告。
 *
 * 内存布局（一次分配的实际块）：
 *
 *   ┌─────────┬──────────┬────────────────┬──────────┐
 *   │sentinel │  size    │  用户数据区     │sentinel  │
 *   │0xAA     │  记录大小 │  ← ptr 返回这里 │0xBB      │
 *   │(1B)     │  (8B)    │  (size 字节)   │(1B)      │
 *   └─────────┴──────────┴────────────────┴──────────┘
 *   ↑                              ↑
 *   malloc 返回的真实起始地址      operator new 返回给用户的地址
 *
 * 释放时：用 ptr 反推回真实起始地址，读哨兵检查。
 */

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <new>

// ─── 哨兵魔数 ───────────────────────────────────────────────────────────
static const unsigned char FRONT_GUARD = 0xAA;   // 前置哨兵
static const unsigned char BACK_GUARD  = 0xBB;   // 后置哨兵

// ═══════════════════════════════════════════════════════════════════════════════
// 带哨兵的 operator new
// ═══════════════════════════════════════════════════════════════════════════════

void* operator new(std::size_t size)
{
    if (size == 0) size = 1;

    // 总大小 = 前哨兵(1B) + size 记录(8B) + 用户数据(size) + 后哨兵(1B)
    std::size_t totalSize =
        sizeof(unsigned char)      // front guard
        + sizeof(std::size_t)      // 存储原始 size
        + size                     // 用户真正要的
        + sizeof(unsigned char);   // back guard

    char* raw = static_cast<char*>(std::malloc(totalSize));
    if (!raw) throw std::bad_alloc();

    // ─── 写入哨兵和 size ────────────────────────────────────────────────
    raw[0] = FRONT_GUARD;                                 // 前哨兵

    std::memcpy(raw + 1, &size, sizeof(std::size_t));     // 记录 size

    raw[1 + sizeof(std::size_t) + size] = BACK_GUARD;      // 后哨兵

    // 返回用户数据区的起始地址（跳过前哨兵和 size）
    return raw + 1 + sizeof(std::size_t);
}

// ═══════════════════════════════════════════════════════════════════════════════
// 带哨兵检查的 operator delete
// ═══════════════════════════════════════════════════════════════════════════════

void operator delete(void* ptr) noexcept
{
    if (!ptr) return;    // 规则 2

    // 反推回 malloc 的真实起始地址
    char* raw = static_cast<char*>(ptr) - 1 - sizeof(std::size_t);

    // ─── 恢复 size ──────────────────────────────────────────────────────
    std::size_t size;
    std::memcpy(&size, raw + 1, sizeof(std::size_t));

    // ─── 检查前哨兵 ─────────────────────────────────────────────────────
    if (raw[0] != FRONT_GUARD) {
        std::cerr << "!!! 前哨兵被破坏 —— 可能发生了向前越界写\n";
        std::cerr << "    地址: " << ptr << "\n";
        std::abort();   // 发现错误，直接终止
    }

    // ─── 检查后哨兵 ─────────────────────────────────────────────────────
    if (raw[1 + sizeof(std::size_t) + size] != BACK_GUARD) {
        std::cerr << "!!! 后哨兵被破坏 —— 可能发生了向后越界写\n";
        std::cerr << "    地址: " << ptr << "\n";
        std::cerr << "    申请大小: " << size << " 字节\n";
        std::abort();   // 发现错误，直接终止
    }

    // 哨兵完好 → 正常释放
    std::free(raw);
}

// ═══════════════════════════════════════════════════════════════════════════════
// 测试
// ═══════════════════════════════════════════════════════════════════════════════

int main()
{
    {
        std::cout << "=== 正常使用：不越界 ===\n";
        int* arr = new int[4];    // 4 个 int = 16 字节（x64）
        arr[0] = 10;
        arr[1] = 20;
        arr[2] = 30;
        arr[3] = 40;
        delete[] arr;             // 哨兵检查通过，正常释放
        std::cout << "正常释放完毕。\n\n";
    }

    {
        std::cout << "=== 错误演示：向后越界写 ===\n";
        char* buf = new char[2];   // 申请 2 字节
        buf[0] = 'A';
        buf[1] = 'B';
        buf[2] = 'C';             // ← 踩了后哨兵！
        delete[] buf;             // 这里会检测到并 abort
    }

    return 0;
}

/*
 * ═══════════════════════════════════════════════════════════════════════════════
 * 为什么这个技术能发现 bug？
 * ═══════════════════════════════════════════════════════════════════════════════
 *
 * 正常分配 buf[2]：
 *   [0xAA][size=2][A][B][0xBB]
 *                      ↑ 后哨兵完好
 *
 * 越界写 buf[2]='C'：
 *   [0xAA][size=2][A][B][ C ]
 *                        ↑ 后哨兵被改写成 'C' (0x43)，不再是 0xBB
 *
 * delete 时检查：
 *   读后哨兵字节 → 0x43 ≠ 0xBB → 报错 abort()
 *
 * 这个技术最适用于：
 *   1. 开发/测试阶段（性能不重要，错误早发现重要）
 *   2. 用 #ifdef _DEBUG 包起来，发布版本切回默认 new/delete
 *   3. 和地址消毒器（AddressSanitizer）互补：ASan 用影子内存，哨兵用嵌入标记
 */
