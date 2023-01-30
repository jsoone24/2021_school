#include <iostream>

using namespace std;

int main()
{
    int array[] = {1,2,3,4,5,6,7,8,9,10,11,22,33,44,55,66,77,88,99,100};
    int maxidx = 0;

    for(int i = 0; i < sizeof(array)/sizeof(int);i++)
        maxidx = array[i] > array[maxidx] ? i : maxidx;
    
    printf("Max element index : %d\n", maxidx);

    return 0;
}