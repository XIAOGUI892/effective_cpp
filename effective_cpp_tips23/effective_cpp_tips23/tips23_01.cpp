class WebBrower {
public:
	void clearCache();
	void clearHistory();
	void removeCookies();
	void removeEverything();//调用上面三个函数
private:

};
void removeEverything(WebBrower& wb) {
	wb.clearCache();
	wb.clearHistory();
	wb.removeCookies();
}
//到底是将removeEverying设置为no-member函数好，还是设置为member1函数好呢？
//理论上，愈少代码能看见类内部的数据，则数据的封装性越好
//从这一点来看，no-member函数更好
//no-member函数还有一个好处，就是降低代码的编译依存
//因为class不可以跨越多个文件，而namespace可以