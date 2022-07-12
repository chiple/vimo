#include <stdio.h>

typedef struct
{
  int x;
  int y;
}Point;


typedef struct
{
  /*maybe key would be y*/
  int key;
  Point value;
  void (*castType)();
}Pair;

int near(Pair* pairs, int length)
{
  for(int i=0; i < length; i++){
    if(pairs[i].key )
    pairs[i]
  }
}

int main(){
  Point dot = {1,2};
  Pair pair[10];
  for(int i=0; i < 10; i++){
    Point tmp = {i+1, i+2};
    pair[i]={i+1,tmp};
  }

  printf("x::%d y::%d", .);
  return 0;
}
