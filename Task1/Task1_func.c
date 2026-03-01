#include "prototypes1.h"


void free_matrix(int **matrix) {
    if (matrix == NULL) {
        return;
    }
    free(matrix[0]);
    free(matrix);
}

int read_matrix(FILE *f, int **array, int *rows, int *columns) {
    int i;
    int j;
    for (i = 0; i < *rows; ++i) {
        for (j = 0; j < *columns; ++j) {
            if (fscanf(f, "%d", &(array[i][j])) != 1){
                printf("Not enough numbers in the matrix!");
                free_matrix(array);
                *array = NULL;
                return -4;
            }
        }
    }

    return 0;
}

int find_needed_row(int **array, int rows, int columns) {
    int i;
    int j;
    int k;
    int candidate;

    for (i = 0; i < rows; ++i) {
        int good_row = 1;

        for (j = 0; j < columns; ++j) {
            candidate = array[i][j];
            for (k = 0; k < rows; ++k) {
                if (k == i) {
                    continue;
                }
                if (candidate <= array[k][j]) {
                    good_row = 0;
                    break;
                }
            }
            if (!good_row) {
                break;
            }
        }

        if (good_row) {
            return i;
        }
    }

    return -1;
}

int **delete_row_and_shrink(int **array, int *rows, int columns, int del_row) {
    int **new_array;
    int i;
    int j;
    int dst = 0;
    //int flag=0;
    //printf("Pointer_f1\n");
    if (del_row < 0 || del_row >= *rows) {
        return array;
    }
    //printf("Pointer_f2\n");
    if (*rows == 1) {
        free_matrix(array);
        *rows = 0;
        return NULL;
    }
    //printf("Pointer_f3\n");
    new_array = (int **)malloc((((*rows)-1))*sizeof(int *) + (*(rows)-1)*columns*sizeof(int));
	new_array[0] = (int *)(new_array + *(rows-1));
	for (i=1; i < *(rows)-1; i++) {new_array[i] = new_array[i-1] + columns;}
	//printf("Pointer_f4\n");
    if (new_array == NULL) {
        return array;
    }
    //printf("Pointer_f5\n");
    for (i = 0; i < *rows; i++) {
        if (i == del_row) {
            
        }
        printf("Pointer_repeat\n");
        for (j = 0; j < columns; ++j) {
            if ((i==1)&&(j==0)) {printf("\n%d %d %d\n", dst, i, j); return new_array;}
            new_array[dst][j] = array[i][j];
            printf("Pointer_inner_repeat\n");
        }
        ++dst;
        //flag=0;
    }
    //return new_array;
    //printf("Pointer_f6\n");
    free_matrix(array);
    *rows -= 1;
    //printf("Pointer_f7\n");
    return new_array;
}

int write_matrix(FILE *f, int **array, int rows, int columns) {
    int i;
    int j;
	
    fprintf(f, "%d %d\n", rows, columns);
    for (i = 0; i < rows; ++i) {
        for (j = 0; j < columns; ++j) {
            fprintf(f, "%d", array[i][j]);
            fprintf(f," ");
        }
        fprintf(f, "\n");
    }

    return 0;
}

int main_function(void) {
    FILE *f_in;
    FILE *f_out;
    int **array;
    int rows = 0;
    int columns = 0;
    int del_row;
    int status;

    f_in = fopen("data.dat", "r");
    if (f_in == NULL) {
        return 1;
    }
    
    if (fscanf(f_in, "%d %d", &rows, &columns) != 2) {
        printf("Couldnt read for rows and columns");
        return -1;
    }
    if (rows <= 0 || columns <= 0) {
        printf("Inavlid input for rows and columns");
        return -2;
    }
    //printf("Pointer1\n");
    array = (int **) malloc (rows*sizeof(int *) + rows*columns*sizeof(int));
	array[0] = (int *)(array + rows);
	for (int i=1; i < rows; i++) {array[i] = array[i-1] + columns;}
	//printf("Pointer2\n");
    status = read_matrix(f_in, array, &rows, &columns);
    fclose(f_in);
    //printf("Pointer3\n");
    if (status != 0) {
        free(array);
        return 2;
    }
    //printf("Pointer4\n");
    del_row = find_needed_row(array, rows, columns);
    //printf("Pointer5\n");
    array = delete_row_and_shrink(array, &rows, columns, del_row);
    return 0;
    //printf("Pointer6\n");
    f_out = fopen("data.res", "w");
    if (f_out == NULL) {
        free_matrix(array);
        return 3;
    }
    //printf("Pointer7\n");
    write_matrix(f_out, array, rows, columns);
    //printf("Pointer8\n");
    fclose(f_out);
    free_matrix(array);
    return 0;
}
