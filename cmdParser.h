#include <string.h>

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
  return !strcmp(formar, latter);
}

int cmdParse(char* cmd, bool saved_flag){
  int result = 0;
  if(intutiveCmp(cmd, "q")){
    result = QUIT;
  }else if(intutiveCmp(cmd, "w")){
    result = SAVE;
  }
  return result;
}
