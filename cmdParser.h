#include <string.h>
#include <stdio.h>

#ifndef CMDPARSER_H_
#define CMDPARSER_H_



#endif // CMDPARSER_H_

#include <stdbool.h>

enum Cmd
  {
    WRITE,
    QUIT,
    BANG,
    SED,
    GREP,
    SAVE
  };

extern enum Cmd cmd;

typedef struct{
  int id;
}cmdParser;


bool intutiveCmp(char* formar ,char* latter)
{
  int len = (int)strlen(latter);
  char new[len];
  for(int i=0; i<len; i++){
    new[i] = formar[i];
  }
  return !strcmp(new, latter);
}



int cmdParse(char* cmd, bool saved_flag){
  int result = 0;

  if(intutiveCmp(cmd, "q")){
    result = QUIT;
  }else if(intutiveCmp(cmd, "w")){
    result = SAVE;
  }else if(cmd[0] == "!"){
    result = BANG;
}
  return result;
}
