#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int ** write_innto_array( f, int **array, int rows, int columns);


int find_needed_string(int **array);
//returns the number  of string that needes to be deleted, if no such string exists, return -1

//If del_num is equal to -1, just return our array

int ** modify_array( int **array, int del_num);
/*"delete" the del_num string by:
1. Making the copy of the array: "int ** array_copy;"
2. Allocate the memory: "array_copy = (int **)malloc((rows-1)*sizeof(int *) + (rows-1)*(columns)*sizeof(int));"
3. Write the modified string of pointers;
4. Write the modified array of numbers;
5. Reallocate memory in array, same as (1);
6. Copy pointers and numbers from array_copy
7. "free(array_copy);" and "return array;"
*/


int main_function(void);
