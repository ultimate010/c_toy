#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum{ false,true} bool;

#define FILEIN  "./input.c"
//������󳤶�
#define MAXKEYLEN 50
//��ຯ������
#define MAXFUN 20
#define MAXKEYWORD 6
#define CODE_NOTFOUND 0
#define CODE_FOUND -6
#define CODE_FUN_DEF -5
#define CODE_FUN_REA -4
/*
+ * �ؼ���
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

int comBegin= 0; //�Ƿ����ע��
int funBegin= 0; //�Ƿ���뺯��
int forBegin = 0; //�Ƿ����forѭ��
int Usedtable= 0; //��ʹ���˶���
char currentFun[MAXKEYLEN]={0}; //��ǰ����ĺ���
int comment(const char * line); //�ж��Ƿ�Ϊע��
void parse(FILE* file,table_t *table); //����
void parseFun( char *buf,table_t *table,FILE *file); //��������
bool isDataType(const char *name); //�ж��Ƿ��ǹؼ���
int getFunName(char *funName,const char *line); //��ú�����
bool isPrint(char *data,const char *line); //�Ƿ��Ǵ�ӡ���
void print(table_t *table); //��ӡ����
void skipSpace(char **buf);


void main()
{

  FILE *fileIn = fopen(FILEIN,"r"); //���ļ�
  if(fileIn==NULL)
  {

    fprintf(stderr,"File not found\n");
    exit(-1);
  }
  table_t table[MAXFUN]; //Ϊ��������
  int i = 0;
  for(;i<MAXFUN;i++)
  {
    table[i].nextFun=-1;
  }
  parse(fileIn,table); //����
 // print(table);
  fclose(fileIn);

}
/*
 * �ж�ע��
 */
int comment(const char * line)
{
  while(*line==' ') line++; //������ʼ�ո�
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
      //��������'//'ʱ��
      return 1;
    }else if(*(line+1)=='*'){
      //'/*'��ʱ��,����ע�Ϳ�ʼ
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
 * ���ұ����Ƿ�����ú������������������λ�ã����ز���λ��
 */
int getTableFun(table_t *table,const char *name){
  int i = 0;
  for(;i<Usedtable;i++){
    if(strcmp(table[i].funName,name)==0){
      return i;
    }
  }
  return Usedtable; //û�ҵ����������һ��λ����һ��
}
/*
 * ��ȡ������,lineָ������������ʼλ��
 */
int getFunName(char *funName,const char *line){
  while(*line=='\t'||*line==' ') line++;
  const char *pCur=line;
  while(*pCur!='(') pCur++;
  strncpy(funName,line,pCur-line);
  while(*pCur!=')') pCur++;
  if(*(pCur+1)==';'){
    return CODE_FUN_DEF; //��������
  }else{
    return CODE_FUN_REA; //����ʵ��
  }
}
/*
 * ���ж��Ƿ��Ǳ������壬ֻ���жϿ�ʼ�Ƿ�Ϊ��������
 */
bool isVariableDef(char * buf){
    skipSpace(&buf);
    if(isDataType(buf) == true){
      return true;
    }
    return false;
}
/*
 * ������������,������򵥵����,ֻ�������ı�������
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
 * �ж��Ƿ���for��ʼ
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
 * ����for��䣬ֻ�����������
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
  //�����Ǹ�����
  if(forBegin == true){
    //��forѭ����
    char printContent[256];
    if(isPrint(printContent,buf) == true){
      printf("In for, print content is %s\n",printContent);
    }else if(isForEnd(buf) == true){
        forBegin = false;
        printf("For is end\n");
    }
  }else{
    //û�н���forѭ��
    if(isVariableDef(buf) == true){
      doVariableParse(buf);
    }else{
      if(isForBeg(buf) == true){
        doFroParse(buf);
        forBegin = true;
      }else{
        //������������ǵ������в�����
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
    //�����
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
    //��һ��,����������
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

