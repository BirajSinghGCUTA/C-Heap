#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

int main()
{
   int *ptr = (int *)malloc(sizeof(int)*10);
   
   *ptr = 10;
   

   int * ptr_new = (int *)realloc(ptr, sizeof(int)*1);
   *ptr_new = 1;
   printf("realloc test PASSED\n");

   return 0;
}