class Date;
Date today();
void clearAppointments(Date d);
//能用声明式，就不用定义式
//原因：如果使用定义式：如使用#include"Date.h"，如果Date.h文件改变，tips_31_02_01.h文件都要改变，导致所有包含该头文件的源文件都要重新编译，浪费时间
