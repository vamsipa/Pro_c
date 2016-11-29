
#include<stdio.h>
#include<math.h>
#include<stdlib.h>

int main()
{
  puts("*****************");
  
  int num[4],
      max;

  
  //get 4 integers as num array
  puts("Enter 4 integers");

  for(int i=0; i<4; i++)
    {
      scanf("%d",&num[i]);

    }

  //assume first ineger as maximum value
  max=num[0];

  //compare max with second integ in array and if greater swap to max to get greatest integer in max
  
  for(int j=1; j<4; j++)
    {
      
      if(num[j]>max)
	max=num[j];
      else
	continue;
	
      
    }
  
  //print max num  
  printf("\n Maximum Integer is : %d",max);


  return 0;
}
