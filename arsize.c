

/*
 *  finding arraysize
 *  arsize.c
 *
 *  Created on: Dec 04, 2016
 *      Author: vamsi
 */


#include<stdio.h>
#include<math.h>
#include<stdlib.h>


///////////// Functions Prototyping





/////////////////
int main()
{

  int num[]= {10,20,30,40,50,50};
  int *ptr=NULL;
      

  puts("number of elements in array are:  ");
  printf("  %d",(sizeof(num)/sizeof(num[0])));

  return 0;
  
}





