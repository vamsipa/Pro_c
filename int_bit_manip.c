

#include<stdio.h>
#include<math.h>
#include<stdlib.h>


///////////// Functions Prototyping
int bin2num(int num);




/////////////////
int main()
{

  int num;
      


  // save the user entered integer in 'num' variable
  printf("\n Enter the Decimal num: ");
  scanf("%d",&num);

  //printf("\n number is :%d",num);

  ///////////////////////////////////////////////////////////////////////////
  int num_bin;

  num_bin=bin2num(num);

  printf("\n 32 bit Binary of Decimal num is : %d",num_bin);

  puts("\n *************************");

  ////////////////////////////////////////////////////////////////////////////

  int c;

  c= num | 15; 

  c=bin2num(c);
  
  printf("\nDecimal Number with all bits set is: %d ",c);


  //////////////////////////////////////////////////////////////////////////////

   
  c= num & 0; 

  c=bin2num(c);
  
  printf("\nDecimal Number with all bits reset is: %d ",c);






  
  return 0;
  
}



int bin2num(int num)
{

  int i=1,
      quo,
      rem;

  int bin=0;
  
  quo=num; //for starting conversion take num as quotient

  printf("\n\t");


  //we need remainders in reverse order until quotient is 1.
  while(quo!=0)  //compute untile end of binary conversion i.e quotient =1 
    {

      rem=quo%2; //calculate reminder for quo by 2 
      quo =  quo/2;  // assign next iteration quo from current quo
      bin=bin+(rem*i); // form binary number by accumulating using weights as 1's, 10's 100's
      i=i*10;          //multiply i by 10 to get weights to form binary num 
    }


  //printf("\n Binary of num is : %d",bin);
  return bin;

}


