/*
 * 需要实现功能跳过注释，输出函数调用关系，并打输出简单打印语句
 * Zaks Wang
 * 2013-5-14
 */
//comment
/*
 * 示例输出为：
 *  In function fun2,print string is :
 *  I am fun2\n
 *  In function fun3,print string is :
 *  I am fun3\n
 *  Function Call hierarchy
 *  main->fun0->fun1->fun3->End
 */

void fun0();
int fun1();
double fun2();
float fun3();
short main();

void main(){
  fun0();
}

void fun0(){
  fun1();
}

void fun1(){
  fun3()；
}

void fun2(){
  printf("I am fun2\n");
}

void fun3(){
  printf("I am fun3\n");
  fun2();
}
