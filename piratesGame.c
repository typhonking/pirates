#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
FILE *fGoodsNames;
FILE *fGoodsPrices;
FILE *fPorts;
FILE *fDistances;
int credit [16];
int debt [16];
int price [10][16];//good; port
int buff;
int basePrice[10];
int location[5];//port from; port to; days travelled; port before gibraltar (or copenhagen if did not stop there); route(0 unless skipping athens or heading to the high seas)
char goods[10][20];
char sGoodToTrade[10];
char course [20];
char ports [16][30];
int hold[10];
int coin = 0;
int ship;
int atSea=0;
int exitGame=0;
int logbook [100];
int newlyArrived=1;
int day=1;

void showLogbook (){
  int x =0;
  char input[20];
  printf ("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n-------  ship's log  -------\n");
  while(logbook[day]!=99){
    printf ("day %d  -  docked at %s\n\n", x+1, ports[logbook[x]]);
    x++;
  }
  printf("\n\n::  save as  ::  Dock  ::\n\n");
  scanf("%s", input);
  if (strcmp(input, "d") == 0) return;
  FILE *fSave = fopen (input, "w");
  fprintf (fSave, "%d %d \n", atSea, newlyArrived);
  fprintf (fSave, "%d %d %d %d %d \n", location[0], location[1], location[2], location[3], location[4]);
  for (int x=0; x<10; x++) fprintf (fSave,"%d ", hold[x]);
  fprintf (fSave, "\n%d %d \n", coin, ship);
  for (int x=0; x<16; x++) fprintf (fSave, "%d %d ", credit[x], debt[x]);
  fprintf(fSave, "\n");
  for (int x=0; x<16; x++) {//prices saved
	for (int y=0; y<10; y++) fprintf (fSave,"%d ", price[y][x]);
  }
  fprintf(fSave,"\n");
  for (int x=0; x<100; x++) fprintf (fSave, "%d ", logbook[x]);
  fclose(fSave);
}

  int plotCourse (char course[]){//returns 0 if course is a port, otherwise returns 1
    location[2]=0;
    for(int x=0;x<16;x++){
	if (strcmp(course, ports[x]) == 0){
        location[1] = x;
        location[2] = 1;
      }
    }
    return location[2]==0?1:0;
  }


  int showGoodsPrices(int y) {
  printf ("--  the market prices in %s  --  ship's hold\n", ports[location[0]]);
    for (int x=0;x<10;x++){
      printf("%s : ", goods[x]);
      printf("%d", price[x][y]);
      printf("              %d\n", hold[x]);
      }
    printf ("                         coin : %d", coin);
    return 0;
    }

void buyGoods (int amount, int goodToTrade){
  int balance =0;
  int purchasingPower = 0;
  balance = amount * price [ goodToTrade ] [ location[0] ];
  purchasingPower = coin + credit [ location [0] ] - debt [ 8 ];
  if ( purchasingPower >= balance ) {
    hold[goodToTrade] += amount;
    coin -= amount*price[goodToTrade][location[0]];
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nbought %d for %d. %s to be delivered before dusk\n\n", amount, amount * price [ goodToTrade ] [location[0]], goods[goodToTrade]);
    if (coin < 0){
      debt[location[0]] += (coin * (-1));
      coin =0;
      printf("extended credit from merchant to value of %d\n\n", debt[location[0]]);
    }
  } else {
    printf ("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nthe merchant will not give so much in advance of payment\n\n");
  }
  getchar();
  getchar();
}

int market();

void sellGoods(int amount, int goodToTrade){
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    if (hold [goodToTrade] >= amount) {
    hold [goodToTrade] -= amount;
    coin += amount * price[goodToTrade][location[0]] - debt[location[0]];
	printf("\n\nsold %d for %d. payment to be given on delivery before dusk\n\n", amount, amount*price[goodToTrade][location[0]]);
  }
  else {
    printf ("\n\ninventory of %s is short\n\n", goods[goodToTrade]);
  }
  getchar();
  getchar();
}


  int market (){
    int goodToTrade;
    char buyOrSell[10];//should be b or s or d
    int amount = 0;
    printf ("\n\n\n\n\n\n\n\n\n\n\n");
    showGoodsPrices (location[0]);//prints list of prices and hold contents
    printf("\n\n::  Buy amount of wares  ::  Sell amount of wares  ::  Dock ::\n\n");
    scanf ("%s", buyOrSell);
    if (strcmp(buyOrSell, "d") == 0) return 0;
    scanf ("%d", &amount);
    scanf ("%s", sGoodToTrade);

    for (int x=0; x<10; x++){
      if ( strcmp(goods[x], sGoodToTrade) == 0){
        goodToTrade = x;
      }
    }
    if (strcmp(buyOrSell, "b") == 0) buyGoods(amount, goodToTrade);
    else if (strcmp(buyOrSell, "s")==0) sellGoods(amount, goodToTrade);
    market();
    return 0;
  }

/* -- not working
void getDistances(int from1; int from2; int from3; int from; int from5){
  char buffer [20];
  int x;
  int number;
  int distance;
  //if (location[0] < 5) {
    fscanf(fDistances,"%s", buffer);
    number = buffer[1];
    if (number < 10){

    }
    printf ("%d", distance);
  //}
}
*/

int setSail(){
  char goTo [20];
  int tryAgain;

  printf("destination  -------  journey length");
  for (int x=0;x<16;x++){
    //getDistances(location);
    printf("%s               1\n", ports[x]);
  }
  rewind (fPorts);
  printf("\n\n::  set course  ::  Dock  ::\n\n");
  scanf("%s", goTo);
  if (strcmp(goTo, "d") == 0) return 1;
  return plotCourse(goTo);//returns 0 if goTo is a port, otherwise returns 1
}

  int setGoodsPrices (){
    for (int y=0; y<16; y++){
      for (int x=0; x<10; x++) {
        price[x][y] = 0;
        }
    }
    for (int y=0; y<16; y++){
      for (int x=0;x<10;x++){
        fscanf(fGoodsPrices,"%d", &basePrice[x]);
        price [x][y] = 50*rand()/RAND_MAX + (rand()/RAND_MAX + 0.5) * basePrice[x];
        }
      }
      rewind (fGoodsPrices);
    return 0;
  }


  void dock(){
    char input[9];
	location[0]=location[1];
    printf("\n\n\n\n\n\n\n\n\n\n\n");
    if (newlyArrived) printf("-------  docked in %s -------", ports[location[0]]);
    else printf("-------  ship is ready to sail from %s  -------", ports[location[0]]);
    newlyArrived=0;
    printf("\n\n\n\n\n\n::  Market  ::  Sail  ::  Hold  ::  Crew  ::  showLogbook  ::\n\n");
    scanf("%s", input);
    if (strcmp(input, "m") == 0) market();
    if (strcmp(input, "q") == 0) exitGame=1;
    if (strcmp(input, "l") == 0) showLogbook();
    if (strcmp(input, "s") == 0) {
      if (setSail() == 0)  atSea=1;
    }
  }


  int sea(){
    newlyArrived=1;
    char goTo[20];
    printf("\n\n\n\n\n\n\n\n\n-._,<._,<._,-  sailing for %s  -._,<._,<._,-\n\n\n\n\n\n", ports[location[1]]);
    printf ("::  Sail on  :: change course  ::");
    scanf ("%s", goTo);
    if (strcmp(goTo,"s") == 0) {
      atSea=0;
    }
    else if (plotCourse(goTo) == 0) atSea=0;
  }

void gameLoop(){
  while (!exitGame){//see if at sea
    if (!atSea) dock();
    else sea();
  }
}
  int newGame(){
      setGoodsPrices ();
      location [0]=7;//set position to porto
      location [1]=7;
      location [2]=0;
	  for (int x=0;x<16;x++) {
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
      for (int x=0;x<16;x++){
        credit [x] = 0;
        debt [x] = 0;
      }
      for (int x=0;x<100;x++){
        logbook[x]=99;
      }
      ship = 1;
      coin = 1000;
      gameLoop();
  }

  int startScreen (){
    char input[10];
    printf(":: N for new game  ::  file name to load  ::");
    scanf("%s", input);
    if (strcmp (input, "n") == 0){
      newGame();
    } else {
      startScreen();
    }
     /*else {
      loadGame(input);
    }*/
    return 0;
  }

int main()
{
  srand (time(0));

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

///  --- FUNCTIONS ---
  //void loadGame(char input[]){
  //  fscanf.........}







// ---- start ---

startScreen();
fclose(fPorts);
fclose(fGoodsNames);
fclose(fGoodsPrices);
  return 0;
}
