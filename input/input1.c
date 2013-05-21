/*
 * 需要实现输出条件语句循环语句关系
 * Zaks Wang
 * 2013-5-18
 */
//comment
/*
 * 示例输出为：
 * 识别main函数定义
 * 进入main函数实现
 * 定义变量a初始值为10
 * 定义变量b初始值为15
 * 进入if语句，条件a<b
 * 如果正确输出The condition is true
 * 如果错误输出The condition is false
 * 进入while循环语句,条件是a>b
 * 如果条件成立输出This is in while
 */
//函数定义
int main();
//函数实现
int main(){
  int a=10;
  int b = 15;
  if(a<b){
    printf("The condition is true\n");
  }else{
    printf("The condition is false\n");
  }
  while(a>b){
    printf("This is in while\n");
  }
}
