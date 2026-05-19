/*═══════════════════════════════════════════════════════════════════════════════
 * 条款 50 理由三：收集使用统计 —— 分配大小直方图
 *═══════════════════════════════════════════════════════════════════════════════
 *
 * 目的：统计程序运行时各种大小的内存分配频率。
 *
 * 比如你发现：
 *     32B:  850000 次  ← 绝大多数是 32 字节分配
 *     64B:   20000 次
 *    128B:    5000 次
 *     ...
 *    1024B:     10 次
 *
 * 你就知道该为 32B 定制池分配器（如前面的 FixedSizePool），
 * 投入产出比最大。
 *
 * 这是"以数据驱动优化"的体现 —— 用统计结果指导你替换哪个大小的 new/delete，
 * 而不是凭直觉猜测。
 */

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <new>
#include <map>
#include <vector>
#include <string>
#include <iomanip>

// ═══════════════════════════════════════════════════════════════════════════════
// 统计数据结构
// ═══════════════════════════════════════════════════════════════════════════════

// 每种大小的分配次数
static std::map<std::size_t, std::size_t> allocationHistogram;
// 累计总分配量
static std::size_t totalBytesAllocated = 0;
// 当前活跃分配量（未释放的）
static std::size_t currentBytesInUse = 0;

// ═══════════════════════════════════════════════════════════════════════════════
// 带统计的 operator new / operator delete
// ═══════════════════════════════════════════════════════════════════════════════

void* operator new(std::size_t size)
{
    if (size == 0) size = 1;

    void* ptr = std::malloc(size);
    if (!ptr) throw std::bad_alloc();

    // ─── 统计 ────────────────────────────────────────────────────────────
    ++allocationHistogram[size];        // 该 size 的分配次数 +1
    totalBytesAllocated += size;
    currentBytesInUse  += size;

    return ptr;
}

void operator delete(void* ptr) noexcept
{
    if (!ptr) return;
    std::free(ptr);
    // 注意：不更新 currentBytesInUse，因为我们不知道 ptr 对应的 size（见注释）
}

// 注意：不重写 delete 时更新 currentBytesInUse 的问题 ——
// 默认 delete 没有记录 size 信息。实际做法需要像 debug_allocator 那样
// 在每个分配块里存 size 才能精确统计。

// ═══════════════════════════════════════════════════════════════════════════════
// 导出统计报告
// ═══════════════════════════════════════════════════════════════════════════════

void dumpAllocationStats()
{
    std::cout << "\n";
    std::cout << "══════════════════════════════════════════════════════\n";
    std::cout << "  内存分配统计报告\n";
    std::cout << "══════════════════════════════════════════════════════\n";

    std::cout << "累计分配总字节数: " << totalBytesAllocated << "\n";
    std::cout << "\n分配大小直方图:\n";
    std::cout << "  大小(B)  │  次数       │  柱状图\n";
    std::cout << "───────────┼─────────────┼──────────────────\n";

    for (const auto& [size, count] : allocationHistogram) {
        std::cout << "  "
                  << std::setw(8) << size << "  │  "
                  << std::setw(10) << count << "  │  ";
        // ASCII 柱状图
        int barLen = static_cast<int>(count / std::max(1UL, count)) * 40;
        for (int i = 0; i < std::min(barLen, 60); ++i) {
            std::cout << '#';
        }
        std::cout << "\n";
    }

    std::cout << "══════════════════════════════════════════════════════\n";
    std::cout << "\n➜ 优化建议:\n";

    // 找出分配次数最多的 size
    std::size_t topSize = 0;
    std::size_t topCount = 0;
    for (const auto& [size, count] : allocationHistogram) {
        if (count > topCount) {
            topCount = count;
            topSize = size;
        }
    }

    if (topCount > 0) {
        std::cout << "  分配最频繁的大小是 " << topSize << "B，"
                  << "出现了 " << topCount << " 次。\n";
        std::cout << "  → 建议为 " << topSize << "B 创建固定大小内存池"
                  << "（见 fixed_size_pool.cpp）\n";
    }
}

// ═══════════════════════════════════════════════════════════════════════════════
// 测试：模拟真实程序的分配模式
// ═══════════════════════════════════════════════════════════════════════════════

class SmallObject {
    int  id;
    char pad[28];   // 凑到 32 字节（x64 下 alignof(max_align_t)=16）
};

class MediumObject {
    int  id;
    char pad[60];   // 凑到 64 字节
};

class LargeObject {
    char data[256];
};

int main()
{
    // 模拟：大量小对象（最常见场景）
    for (int i = 0; i < 100; ++i) {
        int* p  = new int(0);                // 4B × 100
        delete p;
    }
    for (int i = 0; i < 50; ++i) {
        double* d = new double(0.0);          // 8B × 50
        delete d;
    }
    for (int i = 0; i < 30; ++i) {
        SmallObject* s = new SmallObject;      // 32B × 30
        delete s;
    }
    for (int i = 0; i < 10; ++i) {
        MediumObject* m = new MediumObject;    // 64B × 10
        delete m;
    }
    for (int i = 0; i < 5; ++i) {
        LargeObject* l = new LargeObject;      // 256B × 5
        delete l;
    }

    // 模拟：std::string 的典型内部分配
    std::vector<std::string> strings;
    for (int i = 0; i < 20; ++i) {
        strings.push_back("hello world " + std::to_string(i));  // ~16-32B
    }

    dumpAllocationStats();

    return 0;
}

/*
 * ═══════════════════════════════════════════════════════════════════════════════
 * 三个理由的关系
 * ═══════════════════════════════════════════════════════════════════════════════
 *
 *   理由 3（统计）→ 告诉你"该优化哪里"
 *   理由 1（池）  → 在统计指出的热点上实施优化
 *   理由 2（哨兵）→ 在开发阶段确保优化前的代码是正确的
 *
 * 这是一个完整的 workflow：
 *   开发期 → 挂哨兵 new/delete 找 bug
 *   测试期 → 挂统计 new/delete 找热点
 *   优化期 → 对热点大小实现池分配器
 *   发布期 → 切回默认 new/delete（或保留池分配器）
 */
