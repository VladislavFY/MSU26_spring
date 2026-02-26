#include <stdio.h>
#include <stdlib.h>

int **allocate_matrix(int rows, int columns) {
    int **matrix;
    int *data;
    int i;

    matrix = (int **)malloc((size_t)rows * sizeof(int *));
    if (matrix == NULL) {
        return NULL;
    }

    data = (int *)malloc((size_t)rows * (size_t)columns * sizeof(int));
    if (data == NULL) {
        free(matrix);
        return NULL;
    }

    for (i = 0; i < rows; ++i) {
        matrix[i] = data + (size_t)i * (size_t)columns;
    }

    return matrix;
}

static void free_matrix(int **matrix) {
    if (matrix == NULL) {
        return;
    }

    free(matrix[0]);
    free(matrix);
}

int read_matrix(FILE *f, int ***array, int *rows, int *columns) {
    int i;
    int j;

    if (fscanf(f, "%d %d", rows, columns) != 2) {
        return -1;
    }
    if (*rows <= 0 || *columns <= 0) {
        return -2;
    }

    *array = allocate_matrix(*rows, *columns);
    if (*array == NULL) {
        return -3;
    }

    for (i = 0; i < *rows; ++i) {
        for (j = 0; j < *columns; ++j) {
            if (fscanf(f, "%d", &(*array)[i][j]) != 1) {
                free_matrix(*array);
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

    if (del_row < 0 || del_row >= *rows) {
        return array;
    }

    if (*rows == 1) {
        free_matrix(array);
        *rows = 0;
        return NULL;
    }

    new_array = allocate_matrix(*rows - 1, columns);
    if (new_array == NULL) {
        return array;
    }

    for (i = 0; i < *rows; ++i) {
        if (i == del_row) {
            continue;
        }
        for (j = 0; j < columns; ++j) {
            new_array[dst][j] = array[i][j];
        }
        ++dst;
    }

    free_matrix(array);
    *rows -= 1;
    return new_array;
}

int write_matrix(FILE *f, int **array, int rows, int columns) {
    int i;
    int j;
	
    fprintf(f, "%d %d\n", rows, columns);
    for (i = 0; i < rows; ++i) {
        for (j = 0; j < columns; ++j) {
            fprintf(f, "%d", array[i][j]);
            if (j + 1 < columns) {
                fputc(' ', f);
            }
        }
        fputc('\n', f);
    }

    return 0;
}

int main_function(void) {
    FILE *in;
    FILE *out;
    int **array = NULL;
    int rows = 0;
    int columns = 0;
    int del_row;
    int status;

    in = fopen("data.dat", "r");
    if (in == NULL) {
        return 1;
    }

    status = read_matrix(in, &array, &rows, &columns);
    fclose(in);
    if (status != 0) {
        return 2;
    }

    del_row = find_needed_row(array, rows, columns);
    array = delete_row_and_shrink(array, &rows, columns, del_row);
    out = fopen("data.res", "w");
    if (out == NULL) {
        free_matrix(array);
        return 3;
    }
    write_matrix(out, array, rows, columns);
    fclose(out);
    free_matrix(array);
    return 0;
}
