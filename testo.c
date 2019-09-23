#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
char x [20];
FILE *fTestFile;
strcpy(x,"hello");
fTestFile = fopen (x, "w");
fprintf(fTestFile,"%s\n",x);
printf("good");
return 0;
}
