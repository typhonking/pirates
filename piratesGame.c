#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <PDCurses-3.8/curses.h>

#define NO_OF_PORTS 16
FILE *fGoodsNames;
FILE *fGoodsPrices;
FILE *fPorts;
FILE *fDistances;
int credit [NO_OF_PORTS];
int price [10][NO_OF_PORTS];//good; port
int basePrice[10];
int portFrom=7;//porto
int portTo=7;
int daysTravelled=0;
char goods[10][20];
char ports [NO_OF_PORTS][30];
int hold[10];
int coin = 0;
int atSea=0;
int exitGame=0;
int day=1;

void showLogbook (){
  int x =0;
  char input[19];
  printw ("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n-------  ship's log  -------\n");
  printw("\n\n::  save as  ::  Dock  ::\n\n");
  getnstr(input,15);
  sprintf(input, "%s.sav", input);
  if (strcmp(input, "d") == 0) return;
  FILE *fSave = fopen (input, "w");
  fprintf (fSave, "%d \n", atSea);
  fprintf (fSave, "%d %d %d\n", portFrom, portTo, daysTravelled);
  for (int x=0; x<10; x++) fprintf (fSave,"%d ", hold[x]);
  fprintf (fSave, "\n%d\n", coin);
  for (int x=0; x<NO_OF_PORTS; x++) fprintf (fSave, "%d ", credit[x]);
  fprintf(fSave, "\n");
  for (int x=0; x<NO_OF_PORTS; x++) {//prices saved
	for (int y=0; y<10; y++) fprintf (fSave,"%d ", price[y][x]);
  }
  fprintf(fSave,"\n");
  fclose(fSave);
}




int showGoodsPrices(int y) {
printw ("--  the market prices in %s  --  ship's hold\n", ports[portFrom]);
  for (int x=0;x<10;x++){
    printw("%s : %d\t%d\n", goods[x],  price[x][y], hold[x]);
  }
  printw ("                         coin : %d", coin);
  return 0;
  }

void buyGoods (int amount, int goodToTrade){
  int balance =0;
  int purchasingPower = 0;
  balance = amount * price [ goodToTrade ] [ portFrom ];
  purchasingPower = coin + credit [ portFrom ];
  if ( purchasingPower >= balance ) {
    hold[goodToTrade] += amount;
    coin -= amount*price[goodToTrade][portFrom];
    printw("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nbought %d for %d. %s to be delivered before dusk\n\n", amount, amount * price [ goodToTrade ] [portFrom], goods[goodToTrade]);
    if (coin < 0){
      credit[portFrom] -= (coin * (-1));
      coin =0;
      printw("extended credit from merchant to value of %d\n\n", -credit[portFrom]);
    }
  } else {
    printw ("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nthe merchant will not give so much in advance of payment\n\n");
  }
  getch();
}

void sellGoods(int amount, int goodToTrade){
	printw("\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
  if (hold [goodToTrade] >= amount) {
    hold [goodToTrade] -= amount;
    coin += amount * price[goodToTrade][portFrom] + credit[portFrom];
	printw("\n\nsold %d for %d. payment to be given on delivery before dusk\n\n", amount, amount*price[goodToTrade][portFrom]);
  }
  else {
    printw ("\n\ninventory of %s is short\n\n", goods[goodToTrade]);
  }
  getch();
}


void market (){
  int goodToTrade=-1;
  char buyOrSell;//should be b or s or d
  int amount = 0;
  char buff[10];
  printw ("\n\n\n\n\n\n\n\n\n\n\n");
  showGoodsPrices (portFrom);//prints list of prices and hold contents
  printw("\n\n::  Buy amount of wares  ::  Sell amount of wares  ::  Dock ::\n\n");
  do{
  buyOrSell=getch ();
  }while(buyOrSell!='b'&&buyOrSell!='s'&&buyOrSell!='d');
  getch();
  if (buyOrSell == 'd') return 0;
  while(goodToTrade==-1){
    addstr("What would you like to trade?\n");
    getnstr (buff,9);
    for (int x=0; x<10; x++){
      if ( strcmp(goods[x], buff) == 0){
        goodToTrade = x;
      }
    }
  }
  addstr("How much?\n");
  scanw ("%d", &amount);
  if (buyOrSell == 'b') buyGoods(amount, goodToTrade);
  else if (buyOrSell =='s') sellGoods(amount, goodToTrade);
  market();
}



void setSail(){
  char goTo [20];
  printw("destination  -------  journey length\n");
  for (int x=0;x<NO_OF_PORTS;x++){
    printw("%s               1\n", ports[x]);
  }
  rewind (fPorts);
  printw("\n\n::  set course  ::  Dock  ::\n\n");
  getnstr(goTo,sizeof(goTo)-1);
  int i = getPortIndex(goTo);
  if(i!=-1){
    portTo = i;
    daysTravelled = 1;
    atSea=1;
  }
}

int setGoodsPrices (){
  for (int y=0; y<NO_OF_PORTS; y++){
    for (int x=0; x<10; x++) {
      price[x][y] = 0;
      }
  }
  for (int y=0; y<NO_OF_PORTS; y++){
    for (int x=0;x<10;x++){
      fscanf(fGoodsPrices,"%d", &basePrice[x]);
      price [x][y] = 50*rand()/RAND_MAX + (rand()/RAND_MAX + 0.5) * basePrice[x];
      }
    }
    rewind (fGoodsPrices);
  return 0;
}


void dock(){
  char input[10];
  printw("\n\n\n\n\n\n\n\n\n\n\n");
  if (portFrom!=portTo) printw("-------  docked in %s -------", ports[portTo]);
  else printw("-------  ship is ready to sail from %s  -------", ports[portFrom]);
  portFrom=portTo;
  printw("\n\n\n\n\n\n::  Market  ::  Sail  ::  Hold  ::  Crew  ::  showLogbook  ::\n\n");
  getnstr(input,sizeof(input));
  if (strcmp(input, "m") == 0) market();
  if (strcmp(input, "q") == 0) exitGame=1;
  if (strcmp(input, "l") == 0) showLogbook();
  if (strcmp(input, "s") == 0) {
    setSail();
  }
}
int getPortIndex(char *portName){
  for(int x=0;x<NO_OF_PORTS;x++){
    if (strcmp(portName, ports[x]) == 0){
      return x;
    }
  }
  return -1;
}


int sea(){
  char goTo[20];
  printw("\n\n\n\n\n\n\n\n\n-._,<._,<._,-  sailing for %s  -._,<._,<._,-\n\n\n\n\n\n", ports[portTo]);
  printw ("::  Sail on  :: change course  ::");
  getnstr (goTo,sizeof(goTo));
  if (strcmp(goTo,"s") == 0) {
    atSea=0;
  }
  else{
    daysTravelled=0;
    int i = getPortIndex(goTo);
    if(i!=-1){
      portTo = i;
      daysTravelled = 1;
      atSea=0;
    }  
  } 
}

void gameLoop(){
  while (!exitGame){//see if at sea
    if (!atSea) dock();
    else sea();
  }
}
int newGame(){
  setGoodsPrices ();
  portFrom=7;//set position to porto
  portTo=7;
  daysTravelled=0;
  for (int x=0;x<NO_OF_PORTS;x++) {
    fscanf(fPorts,"%s",ports[x]);
  }
  for (int x=0;x<10;x++) {
    fscanf(fGoodsNames,"%s",goods[x]);
  }
  rewind(fPorts);
  rewind(fGoodsNames);
  for (int x=0; x<10; x++) {
    hold [x] = 0;
  }
  for (int x=0;x<NO_OF_PORTS;x++){
    credit [x] = 0;
  }
  coin = 1000;
  gameLoop();
}

void startScreen (){
  char input[10];
  printw(":: N for new game  ::  file name to load  ::");
  getnstr(input,9);
  if (strcmp (input, "n") == 0){
    newGame();
  } else {
    startScreen();
  }
    /*else {
    loadGame(input);
  }*/
}
void initCurses(){
  initscr();
  cbreak();
  scrollok(stdscr, TRUE);
  nodelay(stdscr,FALSE);
  keypad(stdscr, TRUE);
  refresh();
}
int main()
{
  srand (time(0));
  initCurses();
  fGoodsNames = fopen("goods", "r");
  if (fGoodsNames == NULL){
    perror("fGoodsNames Error: ");
    return -1;
  }
  fGoodsPrices = fopen("prices", "r");
  if (fGoodsPrices == NULL){
    perror("fGoodsPrices Error: ");
    return -1;
  }
  fPorts = fopen("ports", "r");
  if (fPorts == NULL){
    perror("fGoodsPrices Error: ");
    return -1;
  }

startScreen();
fclose(fPorts);
fclose(fGoodsNames);
fclose(fGoodsPrices);
endwin();
  return 0;
}
