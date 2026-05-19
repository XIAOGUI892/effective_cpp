/*═══════════════════════════════════════════════════════════════════════════════
 * 条款 50 补充：对齐的隐藏陷阱
 *═══════════════════════════════════════════════════════════════════════════════
 *
 * operator new 返回的指针必须满足特定对齐要求：
 *   - C++11/14/17：至少 alignof(std::max_align_t) 字节对齐
 *     x86-64 上 max_align_t 通常是 16（因为 long double / __m128）
 *   - C++17 起可以通过 operator new(size_t, align_val_t) 指定更大对齐
 *
 * 如果你自己分配（比如池分配器），返回的指针必须是合法对齐的。
 * 不满足 → double / __m128 等类型在你的块上可能触发硬件异常。
 *
 * 示例：
 *   alignof(int)    = 4   （x86-64）
 *   alignof(double) = 8   （x86-64）
 *   alignof(max_align_t) = 16  ← operator new 必须满足至少这个
 */

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <cstring>

// ═══════════════════════════════════════════════════════════════════════════════
// 对齐工具函数
// ═══════════════════════════════════════════════════════════════════════════════

// 将指针向上对齐到 alignment 的倍数
inline void* alignUp(void* p, std::size_t alignment)
{
    // 经典公式：p + alignment - 1，然后向下取整到 alignment 倍数
    // 等同于 ((uintptr_t)p + alignment - 1) & ~(alignment - 1)
    //           前提是 alignment 是 2 的幂

    auto addr = reinterpret_cast<std::uintptr_t>(p);
    auto aligned = (addr + alignment - 1) & ~(alignment - 1);
    return reinterpret_cast<void*>(aligned);
}

// ═══════════════════════════════════════════════════════════════════════════════
// 一个需要 32 字节对齐的类型（比如 AVX 向量）
// ═══════════════════════════════════════════════════════════════════════════════

struct alignas(32) AVXData {
    float values[8];   // 8 × float = 32 字节，要求 32 字节对齐
};

// ═══════════════════════════════════════════════════════════════════════════════
// 演示：池分配器如果不做对齐，可能返回不对齐的地址
// ═══════════════════════════════════════════════════════════════════════════════

// 一个"天真"的池分配器 —— 只是为了演示对齐问题
template<size_t BlockSize>
class NaivePool {
    static char* pool;   // 一整块连续内存
    static size_t offset;
public:
    static void* allocate(size_t size) {
        // 天真地直接返回 pool + offset，不检查对齐
        void* ptr = pool + offset;
        offset += BlockSize;
        return ptr;
        // 问题：pool 可能由 malloc 返回，原本是对齐的。
        //      但如果 BlockSize 不是 alignment 的倍数，
        //      第 2 个、第 3 个 block 就可能不对齐！
    }
    static void init() {
        pool = static_cast<char*>(std::malloc(BlockSize * 256));
        offset = 0;
    }
};

template<size_t S> char*  NaivePool<S>::pool   = nullptr;
template<size_t S> size_t NaivePool<S>::offset = 0;

// ─── 改进版：手动做对齐 ────────────────────────────────────────────────

template<size_t BlockSize, size_t Alignment = 16>
class AlignedPool {
    static char* pool;
    static size_t offset;
public:
    static void* allocate(size_t size) {
        // 手动对齐当前偏移
        void* raw = pool + offset;
        void* aligned = alignUp(raw, Alignment);
        size_t padding = static_cast<char*>(aligned) - static_cast<char*>(raw);
        offset += BlockSize + padding;
        return aligned;
    }
    static void init() {
        pool = static_cast<char*>(std::malloc(BlockSize * 256 + Alignment));
        offset = 0;
    }
};

template<size_t S, size_t A>
char*  AlignedPool<S, A>::pool   = nullptr;
template<size_t S, size_t A>
size_t AlignedPool<S, A>::offset = 0;

// ═══════════════════════════════════════════════════════════════════════════════
// 测试
// ═══════════════════════════════════════════════════════════════════════════════

int main()
{
    std::cout << "=== 当前平台的对齐要求 ===\n";
    std::cout << "alignof(int)           = " << alignof(int)           << "\n";
    std::cout << "alignof(double)        = " << alignof(double)        << "\n";
    std::cout << "alignof(long double)   = " << alignof(long double)   << "\n";
    std::cout << "alignof(max_align_t)   = " << alignof(std::max_align_t) << "\n";
    std::cout << "alignof(AVXData)       = " << alignof(AVXData)       << "\n";

    // 验证 alignUp
    char dummy[64];
    void* raw  = &dummy[3];          // 故意挑一个非对齐地址
    void* up   = alignUp(raw, 16);
    std::cout << "\n未对齐: " << raw << "\n";
    std::cout << "对齐后: " << up  << "  (16 倍数? = "
              << (reinterpret_cast<std::uintptr_t>(up) % 16 == 0
                  ? "yes" : "no") << ")\n";

    // 验证 malloc 返回的地址本身就满足 max_align_t
    void* m = std::malloc(128);
    std::cout << "\nmalloc(128) 对齐检查: "
              << (reinterpret_cast<std::uintptr_t>(m) %
                  alignof(std::max_align_t) == 0
                  ? "OK" : "不对齐!")
              << "\n";
    std::free(m);

    std::cout << "\n➜ 结论：如果你自己写池分配器，\n"
              << "   1. 用 malloc 批一捆：起点已对齐\n"
              << "   2. 每次切块时检查偏移量是不是 alignment 的倍数\n"
              << "   3. 不是 → alignUp 手动对齐\n"
              << "   4. 不满足 → AVX/SSE 指令可能触发硬件异常\n";

    return 0;
}
