/*
 * Zaks Wang
 * 2013-5-14
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FILEIN  "./input0.c"
//函数最大长度
#define MAXKEYLEN 50
//最多函数个数
#define MAXFUN 20
#define MAXKEYWORD 6
#define CODE_FOUND -6
#define CODE_FUN_DEF -5
#define CODE_FUN_REA -4
/*
 * 关键词
 */
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
int Usedtable= 0; //表使用了多少
char currentFun[MAXKEYLEN]={0}; //当前进入的函数
int comment(); //判断是否为注释
void parse(FILE* file,table_t *table); //解析
void parseFun(const char *buf,table_t *table); //解析函数
int isDataType(const char *name); //判断是否是关键字
int getFunName(char *funName,const char *line); //获得函数名
int isPrint(char *data,const char *line); //是否是打印语句
void print(table_t *table); //打印数据


int main(){
  FILE *fileIn = fopen(FILEIN,"r"); //打开文件
  if(fileIn==NULL){
    fprintf(stderr,"File not found\n");
    exit(-1);
  }
  table_t table[MAXFUN]; //为函数建表
  int i = 0;
  for(;i<MAXFUN;i++){
    table[i].nextFun=-1;
  }
  parse(fileIn,table); //解析
  print(table);
  fclose(fileIn);
}
/*
 * 判断注释
 */
int comment(const char * line){
  while(*line==' ') line++; //跳过开始空格
  if(comBegin==1){
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
int isDataType(const char *name){
  int flag=0;
  int i = 0;
  for(;i<MAXKEYWORD;i++){
    if(strcmp(name,KEYWORD[i])==0){
      flag=CODE_FOUND;break;
    }
  }
  return flag;
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

void parseFun(const char *buf,table_t *table){
  const char *pCur,*pPre;
  pCur=pPre=buf;
  char temp[MAXKEYLEN];
  while(*pCur==' '||*pCur=='\t'){
    //跳过前面空格
    pCur++;pPre++;
  }
  int errCode;
  if(funBegin==0){
    //没有进入函数
    while(*pCur!=' '&&*pCur!='\t'){
      pCur++;
    }
    memset(temp,0,MAXKEYLEN);
    strncpy(temp,pPre,pCur-pPre);
    if(isDataType(temp)==CODE_FOUND){
      //是数据类型,下一步是获取函数名
      memset(temp,0,MAXKEYLEN);
      errCode=getFunName(temp,pCur);
      if(errCode==CODE_FUN_DEF){
        //是函数的定义的情况
        //stepOne;查表是否放入该函数声明
        errCode = getTableFun(table,temp);
        if(errCode==Usedtable){
          //没有数据,插入errCode位置
          strcpy(table[errCode].funName,temp);
          Usedtable++;
        }else{
          //已经存在
        }
      }else{
        //是函数实现的情况
        funBegin=1; //函数开始,跳过这句
        strcpy(currentFun,temp);
      }
    }else{
      //这种情况不存在
    }
  }else{
    //进入函数了，识别三种情况，函数调用或者printf语句,或者函数结束
    if(*pCur=='}'){
      funBegin=0;//函三结束
    }else{
      memset(temp,0,MAXKEYLEN);
      if(isPrint(temp,pCur)==CODE_FOUND){
        printf("In function %s,print string is :\n %s\n",currentFun,temp);
      }else{
        memset(temp,0,MAXKEYLEN);
        errCode=getFunName(temp,pCur);
        if(errCode=CODE_FUN_DEF){
          //此时是函数调用
          errCode=getTableFun(table,currentFun);
          int pos=getTableFun(table,temp);
          table[errCode].nextFun=pos;
        }else{
          //不存在
        }
      }
    }
  }
}
int isPrint(char *data,const char *line){
  if(strncmp(line,"printf",6)==0){
    //是输出
    const char *pCur,*pPre;
    pCur=pPre=line;
    while(*pCur!='\"'){
      pCur=++pPre;
    }

    while(*(++pCur)!='\"'){
    }
    strncpy(data,pPre+1,pCur-pPre-1);
    return CODE_FOUND;
  }else{
    return 0;
  }
}

void parse(FILE *file,table_t *table){
  char buf[512]={0};
  while(fgets(buf,512,file)){
    //读一行,并跳过空行
    if(*buf=='\n')
    continue;
    if(comment(buf)==1){
      printf("comment: %s\n",buf);
      continue;
    }else{
      parseFun(buf,table);
    }
  }
}

void print(table_t *table){
    int pos=getTableFun(table,"main");
    printf("Function Call hierarchy\n");
    while(table[pos].nextFun!=-1){
      printf("%s->",table[pos].funName);
      pos = table[pos].nextFun;
    }
    printf("End\n");
}
