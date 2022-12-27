#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "xml.h"
#include <conio.h>
int main()
{

         int r,c,x;
         int parameters[3];
         read_xml(parameters);
         r=parameters[0];
         c=parameters[1];
         x=parameters[2];
      printf("\n\tr:%d\n\tc:%d\n\tx:%d\n",r,c,x);
      getchar();
    return 0;

}
