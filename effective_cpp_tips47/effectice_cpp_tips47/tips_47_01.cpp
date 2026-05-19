//1. 问题引入：std::advance 的设计困境
//STL 中有 5 类迭代器，advance 要把迭代器前进 N 步。不同迭代器的最佳实现不同：

/*┌────────────────┬────────────────────────┬────────────────────────────┐
  │   迭代器类别   │          能力          │    前进 5 步的最佳实现     │
  ├────────────────┼────────────────────────┼────────────────────────────┤
  │ input / output │ 只能单向，每次一步     │ 循环 5 次 ++iter           │
  ├────────────────┼────────────────────────┼────────────────────────────┤
  │ bidirectional  │ 可前进可后退，每次一步 │ 循环 5 次 ++iter 或 --iter │
  ├────────────────┼────────────────────────┼────────────────────────────┤
  │ random access  │ 可任意跳转             │ iter += 5，一次加法搞定    │
  └────────────────┴────────────────────────┴────────────────────────────┘*/
//核心矛盾： 我们希望同一个 advance 函数，对不同迭代器自动选择最优实现。怎么办？

//2. 第一反应：判断迭代器的 "type" 值
#include <vector>
template<typename Iter>
void advance(Iter& iter, int n) {
	if (/*iter是random_access*/) {
		iter += n;
	}
	else {
		for (int i = 0; i < n; i++)++iter;
	}
	/*问题： if 是运行期判断。iter += n 这条语句，
	当迭代器是 list<int>::iterator 时根本编译不过——它不支持 +=。
	你想根据条件只编译其中一个分支，但运行期 if
	做不到，所有分支都必须能正确编译*/
}

//3.转向编译期：利用函数重载
struct input_iterator_tag {};
struct output_iterator_tag {};
// forward 是一种 input，所以继承它（体现"能力递进"）
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

//双向迭代版本：区分正负，逐步移动
template<typename Iter>
void doAdvance(Iter& iter, int n, bidirectional_iterator_tag) {
	if (n > 0) {
		for (int i = 0; i < n; ++i) ++iter;
	}
	else {
		for (int i = 0; i > n; --i) --iter;
	}
}


// 随机访问版本：一次跳到位
template<typename Iter>
void doAdvance(Iter& iter, int n, random_access_iterator_tag) {
	iter += n;
}

 // 单向输入版本：只能前进
template<typename Iter>
void doAdvance(Iter & iter, int n, input_iterator_tag) {
	for (int i = 0; i < n; ++i) ++iter;
}

//调用时，手动传入标签
/*std::vector<int> v = {1,2,3,4};
auto it = v.begin();
doAdvance(it,2,random_access_iterator_tag{})*/
//但这有个问题： 调用者需要自己知道迭代器是什么类别，然后手动传入正确标签。
// 这不实用——调用者只想写 advance(it, 5)，不想管标签。

//4. 核心登场：traits class 自动萃取标签
/*真正的需求是： 写 advance(it, 5) 时，编译器能自动从 Iter 的类型里掏出对应的标签，
然后塞给 doAdvance 做重载派发。
  这就是 traits class 要做的事——它像一个类型上的"萃取机"，
  输入一个迭代器类型，输出它的标签类型*/


//  // traits class：负责从迭代器类型中"榨出"标签类型
template<typename Iter>
struct iterator_traits {
	using iterator_category = typename Iter::iterator_category;
};

//STL 的每种迭代器内部都定义了 iterator_category：

// std::vector 的迭代器内部
class vector_iterator {
public:
	using iterator_category = std::random_access_iterator_tag;
	// ...
};

// std::list 的迭代器内部
class list_iterator {
public:
	using iterator_category = std::bidirectional_iterator_tag;
	// ...
};
/*
  所以 iterator_traits<vector_iterator>::iterator_category → random_access_iterator_tag
  iterator_traits<list_iterator>::iterator_category → bidirectional_iterator_tag*/

/*但有个漏洞： 原生指针 int* 也是随机访问迭代器，但它没有 iterator_category 这个 typedef。
这时候需要用模板偏特化来补上*/

template<typename Iter>
struct iterator_traits<Iter *> {
	using iterator_category = random_access_iterator_tag;
};

// 5. 最终组装：advance 公共接口
//现在把 doAdvance 和 iterator_traits 拼在一起：

template<typename Iter>
void advance(Iter& iter, int n) {
	doAdvance(iter, n, typename iterator_traits<Iter>::iterator_category{});
}