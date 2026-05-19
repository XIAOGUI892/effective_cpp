class AccessLevels {
public:
	int getreadOnly() { return readOnly; }
	int setreadWrite(int value) { readWrite = value; }
	int getreadWrite() { return readWrite; }
	void setwriteOnly(int value) { writeOnly = value; }
private:
	int noAccess;
	int readOnly;
	int readWrite;
	int writeOnly;
};
//讲成员变量设为private，可以细微的划分访问控制权限，如果讲成员变量设置为public，所有成员变量都能被修改，访问