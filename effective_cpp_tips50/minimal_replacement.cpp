/*═══════════════════════════════════════════════════════════════════════════════
 * 条款 50：了解何时替换 new 和 delete —— 最小替换骨架
 *═══════════════════════════════════════════════════════════════════════════════
 *
 * 默认 operator new 内部逻辑（伪代码）：
 *
 *   void* operator new(size_t size) {
 *       if (size == 0) size = 1;           // 零字节请求也保证合法指针
 *       while (true) {
 *           void* ptr = malloc(size);
 *           if (ptr) return ptr;           // ① 成功 → 返回
 *           new_handler h = get_new_handler();
 *           if (h) h();                    // ② 失败有 handler → 调它然后循环
 *           else throw bad_alloc();        // ③ 无 handler → 抛异常
 *       }
 *   }
 *
 *   void operator delete(void* ptr) noexcept {
 *       if (!ptr) return;                  // 对空指针什么都不做
 *       free(ptr);
 *   }
 *
 * 替换 new/delete 时必须遵守的三条规则：
 *   规则 1：new 必须返回正确值（成功 → 指针，失败 → handler 循环 → throw）
 *   规则 2：delete 接到 nullptr 时必须什么都不做
 *   规则 3：new 和 delete 必须成对替换（单对象版配单对象版，数组版配数组版）
 */

#include <cstdlib>
#include <iostream>
#include <new>

// ─── 全局计数器：记录当前程序总分配量 ────────────────────────────────
static size_t totalAllocated = 0;    // 累计分配字节数
static size_t allocationCount = 0;   // 累计分配次数

// ═══════════════════════════════════════════════════════════════════════════════
// 最小替换版本：operator new  +  operator delete
// ═══════════════════════════════════════════════════════════════════════════════

void* operator new(std::size_t size)
{
    // 规则 1 第一步：零字节请求也要返回合法指针
    if (size == 0) {
        size = 1;
    }

    // 规则 1 第二步：循环 —— 失败则调 new-handler，handler 可以腾出内存后返回
    while (true) {
        void* ptr = std::malloc(size);
        if (ptr) {
            // 成功：更新统计信息，返回
            totalAllocated += size;
            ++allocationCount;
            std::cout << "[new]  size=" << size
                      << "  total=" << totalAllocated << "\n";
            return ptr;
        }

        // 失败：取出当前全局 new-handler
        std::new_handler handler = std::get_new_handler();
        if (handler) {
            // 有 handler → 调用它（条款 49 的核心机制）
            // handler 必须做六件事之一：
            //   ① 腾出内存使下次 malloc 成功
            //   ② 安装另一个 handler
            //   ③ 卸载 handler（设为 nullptr）
            //   ④ 抛出 bad_alloc 或其派生类
            //   ⑤ 调用 abort() / exit()
            //   ⑥ 不返回（否则死循环）
            handler();
        } else {
            // 没有 handler → 放弃，抛异常
            throw std::bad_alloc();
        }
    }
}

// 规则 2：接到 nullptr 必须直接返回，不能做任何操作
// 规则 3：这个 delete 配上面的 new（都是单对象版）
void operator delete(void* ptr) noexcept
{
    if (!ptr) return;       // ← 规则 2，必不可少

    std::cout << "[delete]\n";
    std::free(ptr);
}

// ─── 数组版本：逻辑完全相同，只是函数名不同 ──────────────────────────

void* operator new[](std::size_t size)
{
    // 与单对象版相同的逻辑，只是函数签名不同
    if (size == 0) size = 1;

    while (true) {
        void* ptr = std::malloc(size);
        if (ptr) {
            std::cout << "[new[]] size=" << size << "\n";
            return ptr;
        }
        std::new_handler handler = std::get_new_handler();
        if (handler) {
            handler();
        } else {
            throw std::bad_alloc();
        }
    }
}

void operator delete[](void* ptr) noexcept
{
    if (!ptr) return;       // 规则 2
    std::cout << "[delete[]]\n";
    std::free(ptr);
}

// ═══════════════════════════════════════════════════════════════════════════════
// 测试
// ═══════════════════════════════════════════════════════════════════════════════

int main()
{
    std::cout << "=== 单对象分配测试 ===\n";
    int* p = new int(42);
    delete p;

    std::cout << "\n=== 数组分配测试 ===\n";
    int* arr = new int[5];
    delete[] arr;

    std::cout << "\n=== 统计 ===\n";
    std::cout << "分配次数: " << allocationCount << "\n";
    std::cout << "累计字节: " << totalAllocated << "\n";

    return 0;
}
