/*

  Given an integer 'n' , , find each 'x' such that:

  0<=x<=n
  n+x = n^x

where denotes the bitwise XOR operator. Then print an integer denoting the total number of 's satisfying the criteria above.


 */


#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <bitset>
#include <cstdio>
#include <limits>
#include <vector>
#include <climits>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>

using namespace std;


int main(){
    long n,count;
    cin >> n;
    count =0;
    for(long int i=0; i<=n; i++) {
        if((n^i) == (n+i))
            count++;
        else
            continue;
    }

    cout<<count;
    return 0;
}
