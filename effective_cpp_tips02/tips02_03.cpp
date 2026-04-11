class Gameplayer {
private:
	//enum { NumTurns = 5 };//编译常量，编译器会在编译阶段将NumTurns替换为5
	static const int NumTurns = 2;//如果static去掉，就不是编译常量了，编译器无法在编译阶段确定NumTurns的值，无法定义scores数组的大小
	 int scores[NumTurns];//because NumTurns is a compile-time constant, it can be used to define the size of the array
	
};

int fun(int value) {
	const int x = value;
	int arr[x];//报错，因为x不是编译常量，编译器无法在编译阶段确定arr的大小
}