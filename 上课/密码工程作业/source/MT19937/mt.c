#include<stdio.h>
#include"mt.h"
int main()
{
  freopen(NULL, "wb", stdout); 

  int i=387844; 
  init(42);
  while (1) {
       unsigned int value = rng();
      fwrite((void*) &value, sizeof(value), 1, stdout);
    }

  return 0;
}
