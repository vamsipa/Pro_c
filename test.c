#include<stdio.h>




int main(int argc, char *argv[]) {

  int i=0;
  char a[2];
  char b[2];

  puts("Enter string:");
  gets(a);
  puts("Entered string is:");
  puts(a);
  
  puts("Enter string2:");
  scanf("%s",b);
  printf("%s",b);
  printf("\n%c",b[3] );

  
  
  
  return 0;
}

    
