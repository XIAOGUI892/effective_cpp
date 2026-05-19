/*═══════════════════════════════════════════════════════════════════════════════
 * 条款 50 理由一：性能优化 —— 固定大小内存池
 *═══════════════════════════════════════════════════════════════════════════════
 *
 * 通用 malloc 为了处理 1B ~ 1GB 的所有请求，内部有复杂的空闲块管理和锁竞争。
 *
 * 但一个实际程序的内存分配往往不是均匀分布的。比如一个链表程序：
 *   - 绝大多数分配是 sizeof(ListNode)（固定大小）
 *   - 分配/释放极其频繁（插入/删除每个节点都要 new/delete）
 *
 * 如果每次都穿透到 malloc，你付的代价包括：
 *   - malloc 内部的 bin 查找
 *   - 可能触发 brk() 系统调用
 *   - 线程安全锁
 *
 * 这个文件展示池分配器的思想：
 *   ① 一次向系统申请一大块内存（一次系统调用）
 *   ② 在用户态切分成固定大小的槽位
 *   ③ 空闲槽位串成链表（free list）
 *   ④ 分配 = 从链表弹出头部    ← O(1)，无系统调用
 *   ⑤ 释放 = 插回链表头部       ← O(1)，无系统调用
 */

#include <cstdlib>
#include <cstddef>
#include <iostream>
#include <new>
#include <vector>

// ═══════════════════════════════════════════════════════════════════════════════
// 固定大小内存池（极简教学版，不含线程安全和批量申请逻辑）
// ═══════════════════════════════════════════════════════════════════════════════
//
// 内存布局：
//
//   一次向 OS 申请的 chunk（比如 16KB）：
//   ┌──────┬──────┬──────┬──────┬──────┬──────┬─────┐
//   │slot 0│slot 1│slot 2│slot 3│slot 4│slot 5│ ... │
//   │ 32B  │ 32B  │ 32B  │ 32B  │ 32B  │ 32B  │     │
//   └──────┴──────┴──────┴──────┴──────┴──────┴─────┘
//
//   空闲链表（每个空闲 slot 的头 8 字节存 next 指针）：
//   freeList → [slot2] → [slot5] → [slot0] → nullptr
//

template<size_t BlockSize>
class FixedSizePool {
    // 空闲链表的节点：复用空闲 slot 的前几字节存 next 指针
    // 关键技巧：已分配的 slot 不需要 next 指针，
    //         空闲的 slot 用头字节存 next，零额外空间开销
    struct Node {
        Node* next;
    };

    static Node* freeList;   // 空闲链表头指针（类级别共享）

public:
    // 从池里拿一个 slot
    static void* allocate(size_t size)
    {
        // 请求大小必须 <= 池的 slot 大小
        if (size > BlockSize) {
            // 超出池范围的大请求 → 直接走系统 malloc
            return std::malloc(size);
        }

        if (freeList) {
            // 路径 A：空闲链表有货 → O(1) 弹出
            void* ptr = freeList;
            freeList = freeList->next;
            return ptr;
        }

        // 路径 B：空闲链表空了 → 向 OS 批一捆新 slot
        //         思路和 glibc 的 brk() 一样：一次进一批货，慢慢用
        const size_t BATCH_SIZE = 100;
        char* chunk = static_cast<char*>(
            std::malloc(BlockSize * BATCH_SIZE)
        );
        if (!chunk) throw std::bad_alloc();

        // 将这批新 slot 全部串进空闲链表
        for (size_t i = 0; i < BATCH_SIZE; ++i) {
            Node* node = reinterpret_cast<Node*>(
                chunk + (i * BlockSize)
            );
            node->next = freeList;
            freeList = node;
        }

        // 弹出第一个给调用者
        void* ptr = freeList;
        freeList = freeList->next;
        return ptr;
    }

    static void deallocate(void* ptr)
    {
        if (!ptr) return;

        // 插回空闲链表头部（O(1)）
        Node* node = static_cast<Node*>(ptr);
        node->next = freeList;
        freeList = node;
    }
};

// static 成员定义：main 运行前 freeList 为 nullptr
template<size_t S>
typename FixedSizePool<S>::Node* FixedSizePool<S>::freeList = nullptr;

// ═══════════════════════════════════════════════════════════════════════════════
// 使用池的类（比如链表节点）
// ═══════════════════════════════════════════════════════════════════════════════

class ListNode {
    int   val;
    ListNode* next;

public:
    ListNode(int v = 0) : val(v), next(nullptr) {}

    // 重写这个类的 operator new —— 从池里拿，不调全局 malloc
    static void* operator new(size_t size)
    {
        std::cout << "ListNode::operator new(" << size << ")\n";
        return FixedSizePool<sizeof(ListNode)>::allocate(size);
    }

    // 重写 operator delete —— 还给池，不调全局 free
    static void operator delete(void* ptr)
    {
        std::cout << "ListNode::operator delete\n";
        FixedSizePool<sizeof(ListNode)>::deallocate(ptr);
    }

    // 数组版也配对上（规则 3）
    static void* operator new[](size_t size)
    {
        std::cout << "ListNode::operator new[](" << size << ")\n";
        return ::operator new[](size);   // 数组版仍走全局（不做池优化）
    }

    static void operator delete[](void* ptr)
    {
        std::cout << "ListNode::operator delete[]\n";
        ::operator delete[](ptr);
    }
};

// ═══════════════════════════════════════════════════════════════════════════════
// 测试：比较池分配和直接 new/delete 的分配路径
// ═══════════════════════════════════════════════════════════════════════════════

int main()
{
    std::cout << "=== 测试固定大小内存池 ===\n";

    // 创建 1000 个 ListNode
    // 每个 new ListNode：前 100 个触发一批 malloc（补满 freeList）
    //                 后面 900 个直接从 freeList 弹出，零系统调用
    std::vector<ListNode*> nodes;
    for (int i = 0; i < 10; ++i) {
        nodes.push_back(new ListNode(i));
    }
    std::cout << "创建完成，开始释放...\n";

    // 释放：全部插回空闲链表，不调 free
    for (auto* n : nodes) {
        delete n;
    }

    std::cout << "释放完毕。空闲链表里现在有 10 个 slot 可供复用。\n";

    // 再次分配 5 个 —— 直接从 freeList 拿，完全不用 malloc
    for (int i = 0; i < 5; ++i) {
        nodes.push_back(new ListNode(i));
    }

    return 0;
}

/*
 * ═══════════════════════════════════════════════════════════════════════════════
 * 为什么快？
 * ═══════════════════════════════════════════════════════════════════════════════
 *
 *   操作            全局 new/delete                池分配器
 *   ──────────────  ──────────────────────────    ───────────────────────
 *   分配            查 bin → 可能切 chunk          freeList 不为空？
 *                   失败 → brk() 系统调用             是 → 弹出一个指针
 *                   失败 → handler 循环               否 → 批一捆插链表
 *                   成功 → 返回                    返回
 *
 *   释放            找到 chunk 头部                插回 freeList 头部
 *                   插回 bin（可能合并）            O(1)，无合并逻辑
 *                   可能触发 brk 收缩
 *
 *   系统调用         每次分配都可能                只在首次/批次用尽时
 *
 *   锁竞争           每次 malloc 都要获取锁        只有批货时需要锁
 *                   (glibc 有 arenas 缓解)
 *
 *   碎片             通用分配器需要处理任意大小     固定 slot 大小 → 零外部碎片
 *                     请求的混合导致碎片
 */
