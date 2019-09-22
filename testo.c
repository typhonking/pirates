#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
char x [2];
int *y;
int z;
z=0;
y=&z;
//x[1]="9";
strcpy (x,"a");
y=x[2];

printf("%d",y);
return 0;
}
