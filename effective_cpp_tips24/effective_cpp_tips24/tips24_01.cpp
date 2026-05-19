class Rational {
public:
	Rational(int numerator = 0, int denominator = 1);
	int numerator()const;
	int denominator()const;
private:
	int numerator_;
	int denominator_;
};
//问题：如果我想实现两个分数的乘法，我该将operator*函数设置为no-member函数还是friend函数，还是member函数？
//1.设置为member函数
class Rational1 {
public:
	Rational1(int numerator = 0, int denominator = 1);
	int numerator()const;
	int denominator()const;
	Rational1 operator*(const Rational1& other)const {
		return Rational1(this->numerator_ * other.numerator_, this->denominator_ * other.denominator_);
	}
private:
	int numerator_;
	int denominator_;
};//问题在于：（1）Rational r = Rational（1，2） * 2与Rational r = 2 * Rational(1,2) ,前一个正确，后一个错误
//原因：member函数的左操作数必须是this指针绑定的类型


//2.设置为no-member函数
class Rational2 {
public:
	Rational2(int numerator = 0, int denominator = 1);//别加explicit!!!
	int numerator()const;
	int denominator()const;
private:
	int numerator_;
	int denominator_;
};
Rational operator*(const Rational& lhs, const Rational& rhs) {
	return Rational(lhs.numerator() * rhs.numerator(), lhs.numerator() * rhs.denominator());
}
//最佳实践：（1）Rational r = Rational（1，2） * 2与Rational r = 2 * Rational(1,2)全部编译正确，同时no-member函数保证了class的封装性


//3.设置为friend函数
class Rational3 {
public:
	Rational3(int numerator = 0, int denominator = 1);
	int numerator()const;
	int denominator()const;
	friend Rational operator*(const Rational& lhs, const Rational& rhs);
private:
	int numerator_;
	int denominator_;
};
Rational3 operator*(const Rational3& lhs, const Rational3& rhs) {
	return Rational3(lhs.numerator() * rhs.numerator(), lhs.numerator() * rhs.denominator());
}
//其实没必要，设置友元函数目的是访问class成员，但是通过numerator() denominator()接口可以访问成员，friend友元函数没必要，而且友元函数直接访问
//class成员，如果class内部发生变化，要重新检查friend函数的实现，麻烦！！！
//实践中，无论何时，friend函数都应该避免！！！不能够因为函数不能是member函数就把他设置为friend函数
int main() {

}