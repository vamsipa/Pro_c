

/**************************************************
 *  Description:                                  *
 *                                                *
 *  File name:arrmanip.c                          *
 *                                                *
 *  Created on: Dec 04, 2016                      *
 *  Author: Vamsi Pallapothula                    * 
 *              *                                 *                              *  @ Copyrighted All rights reserved             *
 *///**********************************************


#include<stdio.h>
#include<math.h>
#include<stdlib.h>


///////////// Functions Prototyping





/////////////////
int main()
{

  int *arr = NULL,
    num=0;
    

  
  printf(" \n Enter num of Elements of array want to create:");
  scanf("%d",&num);

  
  arr = (int *) malloc(num*sizeof(int));


  if(arr==NULL)
    {

      puts("\n Memory Allocation Failed");
    }


  puts(" Enter the elements ");
  for(int i=0; i<num; i++)
    {
      scanf("%d",&arr[i]);
      
    }


  puts(" Printing array elements ");
  for(int j=0; j<num; j++)
    {
      printf(" \t %d",*(arr+j));

      	
    }

  ///////////////////////////////////////////////////////////////////////////

  puts(" \n Printing array elements after incrementing each by 1 ");

  int *incarr=NULL; //stores new incremented array

  //create memory for as many elements as in original array

  incarr = (int *) malloc(num*sizeof(int));

  
  for(int j=0; j<num; j++)
    {


     incarr[j]= (arr[j])+1;

     printf(" \t %d",*(incarr+j));  
    }

  //////////////////////////////////////////////////////////////////////////////

  //Printing array elements in Even and odd locations seperately
  
  int *evenarr=NULL,  //stores even elements of array
      *oddarr=NULL,   //stores odd elements of array 
         evennum=0,   //stores num of elements presents in even array
           oddnum=0;  //stores num of elements presents in odd array
 
  puts(" \n Printing array elements in Even and odd locations seperately ");


  //loop through full array to divide even and odd locations
  for(int j=0; j<num; j++)
    {

      if(j%2==0) //only if even location element
	{
	  ++evennum; //increment count of elemets in even array

	  //Allocate new int memory for evenarr pointer for every even location 
	  evenarr= (int * ) realloc(evenarr, 1);

	    if(evenarr==NULL)
	    {

	      puts("\nEven Reallo  Memory Allocation Failed");
	    }  

	    else
	      {
		//move array element into new evenarray
		evenarr[j/2]= arr[j];
		
	      }
	    
	}
      else //only odd location
	{
	  ++oddnum; //increment count of elemets in odd array

	      //Allocate new int memory for oddarr pointer for every odd location
	      oddarr= (int * ) realloc(oddarr, 1);

		if(oddarr==NULL)
		{

		  puts("\n Odd Realloc  Memory Allocation Failed");
		}  

		else
		{

		    oddarr[j/2]= arr[j];

		}



	    }
	  //printf(" \t %d",*(arr+j));  
	}



      puts(" \n Printing array elements of even array ");
      for(int j=0; j<evennum; j++)
	{

	  printf(" \t %d",*(evenarr+j));  
        }
  
  
  puts(" \n Printing array elements of odd array ");
  for(int j=0; j<oddnum; j++)
    {

      printf(" \t %d",*(oddarr+j));  
    }


  
  ///////////////////////////////////////////////////////////
  //finding largest element in array.

  puts("\n  **************************** ");

  
  int larg=arr[0],  // variable used to find largest element in array
      largloc=0;
  

  for(int j=1; j<num; j++)
    {

	if(arr[j]>=larg)
	  {
	    larg=arr[j];
	    largloc=j;
	  }
	else
	   continue;

     }
  
  printf("\n Largest element in array is : %d  at %d",larg,largloc);

    //////////////////////////////////////////////////////////












    
    ///////////////////////////////////////////////////////////////////
  free(evenarr);
  free(oddarr);
  free(arr);
  return 0;
  
}





