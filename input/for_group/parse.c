#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum{ false,true} bool;

#define FILEIN  "./input.c"
//函数最大长度
#define MAXKEYLEN 50
//最多函数个数
#define MAXFUN 20
#define MAXKEYWORD 6
#define CODE_NOTFOUND 0
#define CODE_FOUND -6
#define CODE_FUN_DEF -5
#define CODE_FUN_REA -4
/*
+ * 关键词
+ */
static char KEYWORD[MAXKEYWORD][10]={
  "void",
  "int",
  "long",
  "float",
  "short",
  "double"
};
typedef struct{
  char funName[MAXKEYLEN];
  int nextFun;
}table_t;

int comBegin= 0; //是否进入注释
int funBegin= 0; //是否进入函数
int forBegin = 0; //是否进入for循环
int Usedtable= 0; //表使用了多少
char currentFun[MAXKEYLEN]={0}; //当前进入的函数
int comment(const char * line); //判断是否为注释
void parse(FILE* file,table_t *table); //解析
void parseFun( char *buf,table_t *table,FILE *file); //解析函数
bool isDataType(const char *name); //判断是否是关键字
int getFunName(char *funName,const char *line); //获得函数名
bool isPrint(char *data,const char *line); //是否是打印语句
void print(table_t *table); //打印数据
void skipSpace(char **buf);


void main()
{

  FILE *fileIn = fopen(FILEIN,"r"); //打开文件
  if(fileIn==NULL)
  {

    fprintf(stderr,"File not found\n");
    exit(-1);
  }
  table_t table[MAXFUN]; //为函数建表
  int i = 0;
  for(;i<MAXFUN;i++)
  {
    table[i].nextFun=-1;
  }
  parse(fileIn,table); //解析
 // print(table);
  fclose(fileIn);

}
/*
 * 判断注释
 */
int comment(const char * line)
{
  while(*line==' ') line++; //跳过开始空格
  if(comBegin==1)
  {
    if (*line=='*'){
        if(*(line+1)=='/'){
          comBegin=0;
        }
    }
    return 1;
  }
  if(*line=='/'){
    if(*(line+1)=='/'){
      //连续两个'//'时候
      return 1;
    }else if(*(line+1)=='*'){
      //'/*'的时候,表明注释开始
      comBegin=1;
      return 1;
    }
  }
  return 0;
}
bool isDataType(const char *name){
  int i = 0;
  for(;i<MAXKEYWORD;i++){
    if(strncmp(name,KEYWORD[i],strlen(KEYWORD[i]))==0){
      return true;
    }
  }
  return false;
}
/*
 * 查找表中是否包含该函数名，如果包含返回位置，返回插入位置
 */
int getTableFun(table_t *table,const char *name){
  int i = 0;
  for(;i<Usedtable;i++){
    if(strcmp(table[i].funName,name)==0){
      return i;
    }
  }
  return Usedtable; //没找到，返回最后一个位置下一个
}
/*
 * 获取函数名,line指向函数函数名开始位置
 */
int getFunName(char *funName,const char *line){
  while(*line=='\t'||*line==' ') line++;
  const char *pCur=line;
  while(*pCur!='(') pCur++;
  strncpy(funName,line,pCur-line);
  while(*pCur!=')') pCur++;
  if(*(pCur+1)==';'){
    return CODE_FUN_DEF; //函数定义
  }else{
    return CODE_FUN_REA; //函数实现
  }
}
/*
 * 简单判断是否是变量定义，只是判断开始是否为数据类型
 */
bool isVariableDef(char * buf){
    skipSpace(&buf);
    if(isDataType(buf) == true){
      return true;
    }
    return false;
}
/*
 * 解析变量定义,处理最简单的情况,只处理合理的变量定义
 */
void doVariableParse(char *buf){
  skipSpace(&buf);
  while(*buf != ' ') buf++;
  skipSpace(&buf);
  char * pCur = buf;
  while(*pCur != ' ' && *pCur != '=') pCur++;
  char var[16];
  memset(var,0,16);
  strncpy(var,buf,pCur-buf);
  printf("The var name is %s\n",var);
  while(*pCur != '=') pCur++;
  buf = ++pCur;
  skipSpace(&buf);
  pCur = buf;
  while(*pCur != ';' && *pCur != ' ') pCur++;
  strncpy(var,buf,pCur-buf);
  printf("The chushibianling is %s\n",var);
}
/*
 * 判断是否是for开始
 */
bool isForBeg(char *buf){
  skipSpace(&buf);
  if(strlen(buf)<3){
    return false;
  }
  if(buf[0] == 'f' && buf[1] == 'o' && buf[2] == 'r'){
    return true;
  }
  return false;
}

/*
 * 处理for语句，只处理合理的情况
 */
void doFroParse(char *buf){
  skipSpace(&buf);
  buf+=4;
  char *pCur = buf;
  while(*pCur != ';') pCur++;
  char temp[256];
  strncpy(temp,buf,pCur-buf);
  printf("The ini condition is %s\n",temp);
  buf = ++pCur;
  skipSpace(&buf);
  pCur = buf;
  while(*pCur != ';') pCur++;
  strncpy(temp,buf,pCur-buf);
  printf("The condition is %s\n",temp);
  buf = ++pCur;
  skipSpace(&buf);
  pCur = buf;
  while(*pCur != ')') pCur++;
  strncpy(temp,buf,pCur-buf);
  printf("The zengliang is %s\n",temp);
}
bool isForEnd(char *buf){
  skipSpace(&buf);
  if(buf[0] == '}'){
    return true;
  }
  return false;
}


void parseFun(char *buf,table_t *table,FILE *file)
{
  //做你们该做的
  if(forBegin == true){
    //在for循环中
    char printContent[256];
    if(isPrint(printContent,buf) == true){
      printf("In for, print content is %s\n",printContent);
    }else if(isForEnd(buf) == true){
        forBegin = false;
        printf("For is end\n");
    }
  }else{
    //没有进入for循环
    if(isVariableDef(buf) == true){
      doVariableParse(buf);
    }else{
      if(isForBeg(buf) == true){
        doFroParse(buf);
        forBegin = true;
      }else{
        //这种情况在你们的输入中不存在
        printf("I can not parse other condition\n");
      }
    }
  }
}
bool isPrint(char *data,const char *line){
  if(strlen(line)<6){
    return false;
  }
  if(strncmp(line,"printf",6)==0){
    //是输出
    const char *pCur,*pPre;
    pCur=pPre=line;
    while(*pCur!='\"')
	{
      pCur=++pPre;
    }
    while(*(++pCur)!='\"');
    strncpy(data,pPre+1,pCur-pPre-1);
    return true;
  }else
  {
    return false;
  }
}
void skipSpace(char **buf){
  while(**buf == ' ' && **buf != '\0'){
    (*buf)++;
  }
}

void parse(FILE *file,table_t *table)
{
  char line[512]={0};
  char *buf = line;
  while(fgets(buf,512,file))
  {
    //读一行,并跳过空行
    skipSpace(&buf);
    if(*buf=='\n')
    continue;
    if(comment(buf)==1)
	{
      printf("comment: %s\n",buf);
      continue;
    }else
	{
      parseFun(buf,table,file);
    }
  }
}

void print(table_t *table)
{
    int pos=getTableFun(table,"main");
    printf("Function Call hierarchy\n");
    while(table[pos].nextFun!=-1)
	{
      printf("%s->",table[pos].funName);
      pos = table[pos].nextFun;
    }
    printf("End\n");
}

