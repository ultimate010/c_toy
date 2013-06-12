#include <stdio.h>
#include <string.h>
#define G_FILEIN "./input.txt"
void parse(char * line);
void skipSpace(char **buf);
int myatoi(char *buf);
int isVaribleDefine(char *line);
int isConditionline(char *line);
int myatoi(char * buf){
  int result = 0;
  while(*buf!='\0'&&*buf!=';'){
    result*=10;
    result+=*buf-'0';
    buf++;
  }
  return result;
}

int main(){
  FILE * fileIn=fopen(G_FILEIN,"r");
  if(fileIn==NULL){
    printf("Can not open file\n");
    return -1;
  }
  char buf[512];
  memset(buf,0,512);
  while(fgets(buf,512,fileIn)!=NULL){
    parse(buf);
  }
  return 0;

}
void parse(char * line){
  if(isVaribleDefine(line)==1){
  }else{
    if(isConditionline(line)==1){
    }
  }
  printf("line :%s \n",line);
}

int isVaribleDefine(char *line){
  skipSpace(&line);
  if(line[0]=='i' && line[1]=='n' && line[2]=='t'){
    line+=3;
    skipSpace(&line);
    int posSpace=0;
    while(line[posSpace]!=' ' && line[posSpace]!='='){
      posSpace++;
    }
    char name[25];    memset(name,0,25);
    strncpy(name,line,posSpace);
    printf(" The bian ling ming shi %s\n",name);
    line+=(posSpace+1);
    skipSpace(&line);
    line+=1;
    skipSpace(&line);
    int shuzhi = myatoi(line);
    printf("The shuzhi is %d\n",shuzhi);
    return 1;
  }
  return 0;
}
void skipSpace(char **buf){
  while(**buf == ' '&& **buf != '\0'){
    (*buf)++;
  }
}
int isConditionline(char *line){
}
