#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <form.h>
#include "./cmdParser.h"
#include <sys/ioctl.h>

#define size 600000
#define limit_line 1000

enum Cmd cmd;

enum
  {
    ENTER = 10,
    ESC = 27,
    CAPITAL_G = 71,
    BACKSPACE = 127,
  };

enum
  {
    NORMAL,
    INSERT,
    VISUAL,
    COMMAND
  };

typedef struct
{
  /*guestimates*/
  char* key;
  void* value;
  void (*castType)();
}Pair;

typedef struct
{
  int x;
  int y;
}Point;

Point casePoint(void* value)
{
  Point* pointe = (Point*)value;
  return *pointe;
}

Point caseBlank(void* value)
{
  Point* blank= (Point*)value;
  return *blank;
}


typedef struct
{
  int numOfLine;
  int spacePlace;
} DisplayedText;

char *dst[size];
char *bufe[size];
int count = 0;
int mode = NORMAL;
int x = 1;
int y = 1;
int actualX = 1;
int actualY = 1;

int ch;
FIELD *field[3];
FORM *my_form;

Point spacePlace[size];
/*just holding the y of \n */
int blankPlace[size];

bool saved = false;


int currentTop;
int currentBottom;


typedef struct
{
  int width;
  int height;
  /*I'm not sure how much the array can contain*/
  char* buff;
  WINDOW* body;
  //WINDOW* child;
}mainWindow;

void commandMessage(char* message)
{
  mvprintw(LINES-1,4,"%s", message);
}

void saveFile();

void readCommand(char* cmd, bool save_flag)
{
  printw("%d",cmdParse(cmd, save_flag));
  switch(cmdParse(cmd, save_flag)){
  case QUIT:
    endwin();
    exit(2);
    break;
  case SAVE:
    commandMessage("Saved!");
    saveFile();
    break;
  default:
    commandMessage("Not defined");
  }

}

void adjustStatusContents()
{
  printf("asdf");
}


void displayStatusBar();

struct statusBar{
  char* mode;
  char* file_name;
  /*this func for the adjustment between the
    colored box such like NORMAL ln~whatever */
  void (*func)();
  void (*display)();
  /*one of them can be a lsp stuff
    or ln info. */
  char* otherinfo[69];
};

void displayStatusBar(struct statusBar statusbar, int x, int y)
{

  /*somehow, NORMAL is not reflected.*/
  if(mode == NORMAL){
    start_color();
    init_pair(11, COLOR_BLACK, COLOR_CYAN);
    attron(COLOR_PAIR(11));
    mvprintw(LINES -2, 0,  statusbar.mode);
    attroff(COLOR_PAIR(11));
  }
  else {
    start_color();
    init_pair(mode, COLOR_BLACK, mode+2);
    attron(COLOR_PAIR(mode));
    mvprintw(LINES -2, 0,  statusbar.mode);
    attroff(COLOR_PAIR(mode));
  }
  start_color();
  init_pair(10, COLOR_WHITE, COLOR_BLACK);
  attron(COLOR_PAIR(10));
  mvprintw(LINES -2, 10,  statusbar.file_name);
  move(y,x);
  attroff(COLOR_PAIR(10));
}

struct statusBar initStatusBar(char* filename)
{
  struct statusBar statusbar[] = {
    {"NORMAL", /*mode*/
     filename, /*file name*/
     *adjustStatusContents,
    *displayStatusBar},
    {NULL,NULL,NULL}
  };
  return *statusbar;
}

void initCurses()
{
  initscr();
  noecho();
  keypad(stdscr, TRUE);
  cbreak();
}


void bufferMessage(char* text){
  mvprintw(LINES-3 ,0, text);
}

/////////////DISPLAY//////////////
void printNaminami(int line)
{
  for(int linum = 0; linum < LINES-2; linum++)
    {
      if(linum > line-1) mvprintw(linum ,0,"~");
      else{
        start_color();
        init_pair(20, COLOR_YELLOW, COLOR_BLACK);
        attron(COLOR_PAIR(20));
        mvprintw(linum,0,"%3d",linum+currentTop+1);
        attroff(COLOR_PAIR(20));
      }
    }
}

int sequenceSpaces()
{
  int offset = 0;
  return offset;
}

DisplayedText initDisplayedText(char* text)
{
  /* When e key is pressed, vim go to the end of letter of the next word
    or the character that's separated by space from the previous charactor.
   So just take a memo about that. */
  int i = 0;
  int j = 0;
  int numline = 0;
  int tmpX, tmpY = 0;

  DisplayedText displayed;
  while(text[i] != '\0'){
    if(text[i] == ' '){
      Point tmp = {tmpX,tmpY};
      spacePlace[j] = tmp;
      j++;
    }else{;;}
    if(text[i] == '\n'){
      tmpX=0;
      tmpY++;
      numline++;
    }
    tmpX++;
    i++;
  }

  displayed.numOfLine = numline;
  return displayed;
}

void moveToNextSpace()
{
  int left, right = 0;
  /*TODO*/
}


void adjustViewRangePrint(int currentTop, int currentBottom)
{
  int ind=0;
  for(int i=currentTop; i< currentBottom-1; i++){
    mvprintw(ind, 4,"%s", dst[i]);
    ind++;
  }
}



enum
  {
    UP,
    DOWN
  };

void visual(int sx, int sy, int ex, int ey, char* selected)
{
  /*just concat string in the range*/
  int ind=0;
  char strInRange[1024];
  for(int i=sy; i<ey; i++){
      for(int j=sx; j<ex; j++){
        strInRange[ind] = dst[i][j];
        ind++;
      }
    }
  selected = strInRange;
}

void curyBracket(int whichside){
  switch(whichside){
  case UP:

    ;;
  case DOWN:
    ;;
  }
}

bool endOfLine()
{
  if(actualX > (int)strlen(dst[actualY])){
    return true;
  }else{
  return false;
  }
}


void insertBlankLine(int x, int y, int pos)
{
  /*shifting the position of current line*/
  for(int i=count-1; i >  y;i--){
    dst[i+1] = (char*)malloc(sizeof(char) * (int)strlen(dst[i]));
    dst[i+1]=dst[i];
  }
  dst[y+pos] = "\n";
  count++;
}

void deleteLine(int x, int y)
{
  for(int i=y; i < count-1; i++){
    dst[i] = (char*)malloc(sizeof(char) * (int)strlen(dst[i+1]));
    dst[i]=dst[i+1];
  }
  count--;
}

void setMarkedPoints(int **markedpoints, int x, int y)
{
  if(markedpoints[0] == NULL){
    markedpoints[0][0] = x;
    markedpoints[0][1] = y;
  }else{
    markedpoints[1][0] = x;
    markedpoints[1][1] = y;
  }
}


void insertion(int x, int y, char c){
  int X = x-4;
  int len = (int)strlen(dst[y]);

  if(c != '\0'){
    char *new = (char*)malloc(sizeof(char) * (int)strlen(dst[y]) + 1);;
    char *right = (char*)malloc(sizeof(char) * (len - X));
    char *left = (char*)malloc(sizeof(char) * X);
    for(int i=0; i<X; i++) left[i] = dst[y][i];
    for(int i=X; i<len; i++) right[i-X] = dst[y][i];

    sprintf(new, "%s%c%s",left,c,right);
    dst[y] = (char*)malloc(sizeof(char) * (int)strlen(dst[y]) + 1);

    dst[y] = new;
  }else if(c=='\0'){
    char *new = (char*)malloc(sizeof(char) * (int)strlen(dst[y]) - 1);;
    char *right = (char*)malloc(sizeof(char) * (len - X -1));
    char *left = (char*)malloc(sizeof(char) * (X-1));
    for(int i=0; i<X-1; i++) left[i] = dst[y][i];
    for(int i=X; i<len; i++) right[i-X] = dst[y][i];

    sprintf(new, "%s%s",left,right);
    dst[y] = (char*)malloc(sizeof(char) * (int)strlen(dst[y]) - 1);

    dst[y] = new;
  }

}


/*time to update is when the content of the file is written */
void updateSpaces()
{
  int indSpace=0;
  int indBlank=0;
  for(int i=0; i<count-1; i++){
    for(int j=0; j<(int)strlen(dst[i]); j++){
      if(strcmp(dst[i], "\n")){
        blankPlace[indBlank] = i;
        indBlank++;
      }
      else if(dst[i][j] == ' '){
        spacePlace[indSpace].x = j;
        spacePlace[indSpace].y = i;}
      indSpace++;
    }
  }
}

void middleJump(int x, int y)
{

}

void movePerWords()
{

}
/*set the mode NORMAL as default*/
/////////////MODE//////////////
void normalMode(struct statusBar statusbar, int maxLine, int endOfLine)
{
  statusbar.display(statusbar, x, y);
  switch(getch()){

    /*basic movements*/
  case 'j':
    if(y < maxLine) y++;else{y=maxLine-1;}
    if(actualY >= maxLine){
      actualY=maxLine-1;}
    else{
      actualY++;} break;
  case 'k':
    if(y > 0){y--;actualY--;}
    else if(actualY < 1){
      actualY=0;
    }else{
      actualY--;
    }break;
  case 'l':
    actualX++;
    x++;
    break;
  case 'h':
    x--;
    actualX--;
    break;
    /*move per word */
  case 'e': x++;
    x+=3;
    break;
  case 'b': x++; actualX++;
    x+=-3;
    break;


    /* move top or bottom */
  case CAPITAL_G:
    y += maxLine - y -1;
    actualY = maxLine;
    if(actualY > LINES + 2){
      currentTop = actualY - LINES +2;
      currentBottom = actualY;
    }else{;;}
    break;
  case 'g':
    if(getch() == 'g'){
      y=0;
      actualY=0;
      currentTop = 0;
      currentBottom= LINES-2;
      break;}

    /*move to the last or first of sentence*/
  case '$':
    x+=endOfLine;
    actualX+=endOfLine;
    break;
  case '^':
    x=3;
    break;

    /*switching modes*/
    ///////INSERTION////////
  case 'i':
    statusbar.mode= "INSERT";
    mode= INSERT;
    break;
  case 'x':
    insertion(actualX,actualY,'\0');
    x--;
    actualX--;
    break;
  case 'd':
    if(getch() == 'd'){
      deleteLine(actualX, actualY);
    }
    break;

  case 'a':
    x++;
    actualX++;
    statusbar.mode= "INSERT";
    mode= INSERT;
    break;
  case 'o':
    statusbar.mode= "INSERT";
    y++;
    actualY++;
    insertBlankLine(actualX, actualY, 1);
    mode=INSERT;
    break;
    ///////VISUAL////////
  case 'v':
    statusbar.mode= " VISUAL ";
    mode= VISUAL;
    break;
  case ':':
    mode = COMMAND;
    statusbar.mode= " COMMAND ";
    break;
  }
}

void insertMode(struct statusBar statusbar)
{
  statusbar.display(statusbar, x, y);
  int c=getch();
  switch(c){
  case KEY_DOWN: y++;  break;
  case KEY_LEFT: x--; break;
  case KEY_UP: y--; break;
  case KEY_RIGHT: x++; break;
  case ESC:
    mode = NORMAL;
    break;
  case ENTER:
    y++;
    actualY++;
    insertBlankLine(actualX, actualY, 0);
    break;
  case BACKSPACE:
    insertion(actualX,actualY,'\0');
    x--;
    actualX--;
    break;
  default:
    insertion(actualX,actualY,c);
    x++;
    actualX++;
    break;
  }
}


void visualMode(struct statusBar statusbar, int points)
{
  statusbar.display(statusbar, x, y);

  switch(getch()){
  case 'j': y++;  break;
  case 'h': x--; break;
  case 'k': y--; break;
  case 'l': x++; break;
  case ':':
    mode = COMMAND;
    statusbar.mode= " COMMAND ";
    break;
  case ESC:
    mode = NORMAL;
    break;
  }
}



void hundleCommand(struct statusBar statusbar, char* message, int line)
{
  field[0] = new_field(1, 40, LINES-1, 1, 0, 0);
  field[1] = NULL;
  int buf_ind = 0;
  char buf[69];
  /* Set field options */
  field_opts_off(field[0], O_AUTOSKIP);  /* Don't go to next field when this */
  /* Create the form and post it */
  my_form = new_form(field);
  post_form(my_form);

  mvprintw(LINES-4, 2, "%c",dst[y][x-4]);
  printNaminami(count-2);
        for(int i=0; i<count; i++){
          mvprintw(i,4, "%s", dst[i]);
        }
  refresh();
  statusbar.mode = " COMMAND ";
  statusbar.display(statusbar, x, y);
  mvprintw(LINES-1,0,":");

  while((ch = getch()) != ESC)
    {
      printNaminami(line);
        for(int i=0; i<count; i++){
          mvprintw(i,4, "%s", dst[i]);
        }
        mvprintw(LINES-4, 2, "%c",dst[y][x-4]);
        printNaminami(count);


      switch(ch)
        {
        case ENTER:
          readCommand(buf,saved);
        case ESC:
          mode = NORMAL;
          break;
        default:
          /* If this is a normal character, it gets */
          /* Printed  */
          form_driver(my_form, ch);
          buf[buf_ind] = ch;
          buf_ind++;
          break;
        }
    }
  mode = NORMAL;
  statusbar.mode = "NORMAL";
}

void saveFile()
{
  char path[100] = "./testout.txt";
  FILE* fps;
  fps=fopen(path, "w+");
  for(int i=0; i<count-1; i++){
    fprintf(fps,"%s%s", dst[i],"\n"); //save text to file
  }
  fclose(fps);//close file
  printw("saved!");
}

/*this will become the status bar content*/
void printDebug()
{
  mvprintw(LINES-3, 4,"actualX:%d actualY:%d", actualX, actualY);
  mvprintw(LINES-4, 4,"x:%d y:%d", x, y);
  mvprintw(LINES-5, 4,"currentTop:%d currentBotoom:%d", currentTop , currentBottom);
  mvprintw(LINES-6, 4,"maxLine::%d", count);
  mvprintw(LINES-7, 4,"Line on cursor::%s", dst[actualY]);
  mvprintw(LINES-8, 4,"SpacePlace-x::%d -y::%d", spacePlace[0].x, spacePlace[0].y);
}

void limitScreen(int max_x, int max_y)
{
  if ( actualX > max_x ) {
    actualX = max_x;
    x = max_x;
    mvprintw(LINES-1,0,"End of Line");
  }
  else if ( y > max_y-1 ) y = max_y;
  else if ( y < 0 ) y = 0;
  else if ( x < 4 || actualX < 4) {
    x = 4;
    actualX = 4;
    mvprintw(LINES-1,0,"Beginning of Line");
  }

  //for the end of the line
}


int isDelimiter(char p, char delim){
  return p == delim;
}

int split(char *dst[], char *src, char delim){
  char* tmp = "";
  for(;;) {
    while (isDelimiter(*src, delim)){
      src++;
      dst[count++] = "\n";
    }

    if (*src == '\0') break;

    dst[count++] = src;

    while (*src && !isDelimiter(*src, delim)) {

      tmp = src;
      src++;
    }

    if (*src == '\0') break;

    *src++ = '\0';
  }
  return count;
}

void justPrint()
{
  for(int i=0; i<count; i++){
        mvprintw(i,4, "%s", dst[i]);
      }
}

void makeDraft()
{
  for(int i =0; i<size; i++){
    for(int j=0; j<limit_line; j++){

    }
  }
}

int main(int argc, char *argv[])
{

  /*this is for the visual mode, containing the
   x and y of start and end point*/
  int marked[2][2];

    if(argc < 2)
    {
      puts("too few args");
      return 1;
    }

  ////////////FILE///////////////

  char filename[64];
  sprintf(filename,"%s",argv[1]);
  FILE *fp;
  fp = fopen(filename, "r+");

  int maxchars=size/*number of characters allowed to be opened*/,startrow=0/*cursor start row*/,startcol=0/*cursor start column*/;
  char *message=malloc(maxchars*sizeof(char));  //allocate memory for our message string
  int i=0;/*just to count*/
  char c;/*just to read a character from file */

  if(fp!=NULL)/*if fp is pointing to a file*/
    {
      while((c=fgetc(fp))!=EOF)/*keep getting a charcetr from file unless it reaches end of file*/
        {message[i]=(char)c;i++;}/*add the charcetr to data structure message*/
      fclose(fp);/*close file when done traversing*/
    }
  fclose(fp);

  int max_y, max_x;               // max coordinates we can haev on scren
  getmaxyx(stdscr, max_y, max_x); // get maximum coordinates

  /////////////INIT//////////////
  initCurses();

  struct statusBar statusbar = initStatusBar(filename);

  /*TODO this will be updated so this place is wrong to put.*/
  DisplayedText displayed = initDisplayedText(message);

  statusbar.display();

  WINDOW *mwinBody = subwin(stdscr, LINES-2, 100, 0, 2);
  mainWindow mwindow =
    {
      max_x-1,
      LINES-2,
      /*I'm not sure how much the array can contain*/
      message,
      mwinBody,
    };

  currentTop = 0;
  currentBottom = LINES-2;

  Pair Spaces[10] ;

  //Spaces[0].key = "unpoi";
  //Spaces[0].value = (void*){1,2};
  //Point a = Spaces[0].castType(Spaces[0].value);
  /*count of the lines number contained by the target file*/
  int count;
  count = split(dst, message, '\n');
    while(1)
    {
      /*update and adjust the range of the screen to display */
      if(actualY > currentBottom){
        currentTop+=1;
        currentBottom+=1;
      }else if(actualY < currentTop){
        currentTop-=1;
        currentBottom-=1;
      }
      if(actualY > count-1) {actualY=count-1;}else{}

      /*clear the screen*/
      erase();

      //printDebug();

      /*check the range of view depending on the current cursor.*/
      if(y > LINES-3){y=LINES-3;}else{;}

      if(y > currentBottom){ currentTop++; currentBottom++;}
      else if(actualY < currentTop) {currentTop--; currentBottom--;}

      if(count > LINES-2){
        //printInView(message);
        adjustViewRangePrint(currentTop, currentBottom);
      }else{justPrint();}

      printNaminami(count);

      limitScreen(strlen(dst[actualY]) + 3, displayed.numOfLine-1);
      move(y,x);


      switch(mode){
      case NORMAL:
        statusbar.mode= " NORMAL ";
        normalMode(statusbar,count, strlen(dst[actualY]));
        break;

      case INSERT:
        insertMode(statusbar);
        statusbar.mode= " INSERT ";
        break;
      case VISUAL:
        visualMode(statusbar, marked);
        statusbar.mode= " VISUAL ";
        break;

      case COMMAND:
        hundleCommand(statusbar, message, displayed.numOfLine);
        statusbar.mode= " COMMAND ";
        break;
      }
      refresh();

    }
  clear();  // once returned from texteditor clear the screen and buffer
  endwin(); // end window
  unpost_form(my_form);
  free_form(my_form);
  free_field(field[0]);
  free_field(field[1]);
  free(dst);

  return 0;
}
