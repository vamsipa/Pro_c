/*

  You will be given a list of 32 bits unsigned integers. You are required to output the list of the unsigned integers you get by flipping bits in its binary representation (i.e. unset bits must be set, and set bits must be unset).

 */
xs
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

int main() {

    /* Enter your code here. Read input from STDIN. Print output to STDOUT */ 
    int n;
    
    scanf("%d",&n);
    unsigned int arr[n];
    
    for(int i=0; i<n; i++) {
        scanf("%u",&arr[i]);
    }
    
    for(int i=0; i<n; i++) {
        printf("%u\n",(~arr[i]));
    }
    
    
    return 0;
}
