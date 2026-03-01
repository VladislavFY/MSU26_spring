#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void free_matrix(int **matrix);
int read_matrix(FILE *f, int **array, int *rows, int *columns);
int find_needed_row(int **array, int rows, int columns);
int **delete_row_and_shrink(int **array, int *rows, int columns, int del_row);
int write_matrix(FILE *f, int **array, int rows, int columns);
int main_function(void);
