class Estimate {
private:
	static const int cnt = 0;
};
//const int Estimate::cnt = 0; //多次初始化会报错，error: redefinition of ‘const int Estimate::cnt’
int main() {
	Estimate e;
}