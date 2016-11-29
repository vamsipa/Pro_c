

#include<stdio.h>
#include<math.h>

int main()
{

  int num,
      i=1,
      bin=0,
      quo,
      rem;

  printf("\n Enter the num: ");
  scanf("%d",&num);

  printf("\n number is :%d",num);

  //printf("\n num after setting all bits: %x",(num|1));

  quo=num;

  printf("\n\t");
  
  while(quo!=0)
    {

      rem=quo%2;
      quo =  quo/2;
      bin=bin+(rem*i);
      i=i*10;
    }


  printf("\n Binary of num is : %d",bin);

  return 0;
  
}

  
