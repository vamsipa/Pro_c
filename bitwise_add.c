#include<stdio.h>

void add(unsigned int, unsigned int);


int main(int argc, char *argv[]) {

  char a[4];
  // add(3,1);


  a="hel";

  printf("\n%s",a);
  
  return 0;
}

    
void add(unsigned int t1, unsigned int t2)
{
  static int sum,car;
  while(t2!=0)
  {
    car=t1&t2;
    t1=t1^t2;
    t2=car<<1;
  }
  printf("\n%d",t1);


  
}
