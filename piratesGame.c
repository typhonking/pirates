#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
void market(){};
void dock(){};


int main()
{
  srand (time(0));
  FILE *fGoodsNames;
  FILE *fGoodsPrices;
  FILE *fPorts;
  FILE *fDistances;
  int credit [16];
  int debt [16];
  int price [10][16];//good; port
  int buff;
  int basePrice[10];
  int x=0; int y=0; int z=0;
  int a; int b; int c;
  int from1; int from2; int from3; int from4; int from5;
  int location[5];//port from; port to; days travelled; port before gibraltar (or copenhagen if did not stop there); route(0 unless skipping athens or heading to the high seas)
  char goods[100];
  char sGoodToTrade[10];
  char localPort[15];
  char destination[20];
  char course [20];
    int goodToTrade;
    int amount;
  int hold[10];
  int coin = 0;
  int ship;
  int log [100];
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

int logbook (){
  int day =1;
  char portLog [20];
  printf ("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n-------  ship's log  -------\n");
  while (log[day] < 99){
    for (y=0;y<log [day];y++){
      fscanf (fPorts, "%s", portLog);
    }
    rewind(fPorts);
    printf ("day %d  -  docked at %s\n\n", day, portLog);
    day++;
  }
  return 3;
}

  int plotCourse (char course[]){//returns 0 if course is a port, otherwise returns 1

    for(x=0;x<16;x++){
      fscanf(fPorts,"%s", destination);
      if (strcmp(course, destination) == 0){
        location[1] = x;
        location[2] = 1;
        return 0;
      }
    }
    rewind (fPorts);
    return 1;
  }


  int showGoodsPrices(int y) {
  printf ("--  the market prices in %s  --  ship's hold\n", localPort);
    for (x=0;x<10;x++){
      fscanf(fGoodsNames,"%s", goods);
      printf("%s : ", goods);
      printf("%d", price[x][y]);
      printf("              %d\n", hold[x]);
      }
    rewind (fGoodsNames);
    printf ("                         coin : %d", coin);
    return 0;
    }

void buyGoods (int amount, int goodToTrade){
  int balance =0;
  int purchasingPower = 0;
  balance = amount * price [ goodToTrade ] [ location[0] ];
  purchasingPower = coin + credit [ location [0] ] - debt [ 8 ];
  if ( purchasingPower > balance ) {
    hold[goodToTrade] += amount;
    coin -= amount*price[goodToTrade][location[0]];
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nbought %d for %d. goods to be delivered before dusk\n\n", amount, amount * price [ goodToTrade ] [location[0]]);
    if (coin < 0){
      debt[location[0]] += (coin * (-1));
      coin =0;
      printf("extended credit from merchant to value of %d\n\n", debt[location[0]]);
    }
    getchar();
  } else {
    printf ("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nthe merchant will not give so much in advance of payment\n\n");
    getchar();
  }
}


int sellGoods(int amount, int goodToTrade){
    if (hold [goodToTrade] >= amount) {
    hold [goodToTrade] -= amount;
    coin += (price[goodToTrade][location[0]] - debt[location[0]]);
  }
  else {
    printf ("\n\ninventory of %s is short\n\n", sGoodToTrade);
  }
  market();
  return 0;
}


  int market (){
    char buyOrSell[10];//should be b or s or d
    amount = 0;
    printf ("\n\n\n\n\n\n\n\n\n\n\n");
    showGoodsPrices (location[0]);//prints list of prices and hold contents
    printf("\n\n::  Buy amount of wares  ::  Sell amount of wares  ::  Dock ::\n\n");
    scanf ("%s", buyOrSell);
    if (strcmp(buyOrSell, "d") == 0) return 0;
    scanf ("%d", &amount);
    scanf ("%s", sGoodToTrade);

    for (x=0; x<10; x++){
      fscanf(fGoodsNames, "%s", goods);
      if ( strcmp(goods, sGoodToTrade) == 0){
        goodToTrade = x;
        x=9;
      }
    }
    rewind (fGoodsNames);
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
  for (x=0;x<16;x++){
    fscanf (fPorts, "%s", destination);
    //getDistances(location);
    printf("%s               1\n", destination);
  }
  rewind (fPorts);
  printf("\n\n::  set course  ::  Dock  ::\n\n");
  scanf("%s", goTo);
  if (strcmp(goTo, "d") == 0) return 1;
  return plotCourse(goTo);//returns 0 if goTo is a port, otherwise returns 1
}

  int setGoodsPrices (){
    for (y=0; y<16; y++){
      for (x=0; x<10; x++) {
        price[x][y] = 0;
        }
    }
    for (y=0; y<16; y++){
      for (x=0;x<10;x++){
        fscanf(fGoodsPrices,"%d", &basePrice[x]);
        price [x][y] = 50*rand()/RAND_MAX + (rand()/RAND_MAX + 0.5) * basePrice[x];
        }
      }
      rewind (fGoodsPrices);
    return 0;
  }


  int dock(int c){
    char input[9];
    a= location[0];
    for (y=0; y<a; y++){
      fscanf (fPorts, "%s", localPort);
    }
    rewind (fPorts);
    printf("\n\n\n\n\n\n\n\n\n\n\n");
    if (c==3) printf("-------  ship is ready to sail from %s  -------", localPort);
    else printf("-------  docked in %s -------", localPort);
    printf("\n\n\n\n\n\n::  Market  ::  Sail  ::  Hold  ::  Crew  ::  Logbook  ::\n\n");
    scanf("%s", input);
    if (strcmp(input, "m") == 0) market();
    if (strcmp(input, "q") == 0) return 1;
    if (strcmp(input, "l") == 0) return logbook();
    if (strcmp(input, "s") == 0) {
      b = setSail();
      if (b == 0)  return 2;
      else return 3;
    }

    return 3;
  }


  int atSea(){
    char goTo[20];
    for (x=0; x<location[1]; x++){
      fscanf (fPorts, "%s", localPort);
    }
    rewind (fPorts);
    printf("\n\n\n\n\n\n\n\n\n-._,<._,<._,-  sailing for %s  -._,<._,<._,-\n\n\n\n\n\n", localPort);
    printf ("::  Sail on  :: change course  ::");
    scanf ("%s", goTo);
    if (strcmp(goTo,"s") == 0) return 0;
    if (plotCourse(goTo) == 0) return 0;
    return 2;
  }


  int newGame(){
      setGoodsPrices ();
      location [0]=8;//set position to porto
      location [1]=8;
      location [2]=0;
      for (x=0; x<10; x++) {
        hold [x] = 0;
      }
      for (x=0;x<16;x++){
        credit [x] = 0;
        debt [x] = 0;
      }
      for (x=0;x<100;x++){
        log[x]=99;
      }
      ship = 1;
      coin = 1000;
      c=3;
      while (c>0){//see if at sea
      if (c==0) c = dock(c);
      if (c==1) return 0;
      if (c==2) c = atSea ();
      if (c==3) c = dock(c);
    }
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





// ---- start ---

startScreen();
fclose(fPorts);
fclose(fGoodsNames);
fclose(fGoodsPrices);
  return 0;
}
