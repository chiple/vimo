#include <string.h>

#ifndef CMDPARSER_H_
#define CMDPARSER_H_



#endif // CMDPARSER_H_

#include <stdbool.h>

enum
  {
    WRITE,
    QUIT,
    BANG,
    SED,
    GREP
  };


typedef struct{
  int id;
}cmdParser;



int cmdParse(char* cmd, bool saved_flag){
  int result = 0;
  if(strcmp(cmd,"q")){
    result = QUIT;
  }
  return result;
}
