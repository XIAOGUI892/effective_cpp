class Rational {
public:
	Rational(int numerator = 0, int denominator = 1) :n(numerator), d(denominator) {};
private:
	int n, d;
	friend const Rational& operator*(const Rational& lhs, const Rational& rhs) {
		return Rational(lhs.n * rhs.n, lhs.d * rhs.d);
	}
	friend bool operator==(const Rational & lhs, const Rational & rhs) {
		return lhs.n == rhs.d && lhs.n == rhs.n;
	}
		//注意，返回的rational对象在operator函数结束后就销毁了，造成悬空引用
		//怎么解决呢
	//1.在堆空间是构造一个rational对象
		/*friend const Rational& operator*(const Rational & lhs, const Rational & rhs) {
			Rational* res = new Rational(lhs.n * rhs.n, lhs.d * rhs.d);
			return *res;
		}*/
		//问题：怎么delete堆空间呢
		//Rational a,b,c;
		//Rational res = a*b*c;//调用两次new，需要两次delete，但我们无法获得指针来delete
		//====================================================================================
		//2.在operator*函数内部使用static对象
		/*friend const Rational &operator*(const Rational& lhs,const Rational &rhs){
		    static Rational res ;
		    res = Rational(lhs.n*rhs.n,lhs.d*rhs.d)
		    return res;
		}*/
		//问题Rational成为多线程下的共享资源，要解决竞态
		//假设有如下代码
		//if((a*b)==(c*d)){cout<<....;}
		//else {cout<<....;}
		//由于返回的是引用，尽管在函数内部对static成员做了修改，但返回的是引用，故进行==比较时比较的都是static对象的当前状态，结果永远为true
		//=======================================================================================================

		//3.返回值，运用值传递
		/*friend const Rational operator*(const Rational &lhs,const Rational &rhs){
			return Rational(lhs.n*rhs.n,lhs.d*rhs.d)
		}*/
		//牺牲了些许运行速度，但是换来了结果正确
		//实际上编译器会对值传递进行优化，避免了值传递过程中大量的函数调用
	}
};