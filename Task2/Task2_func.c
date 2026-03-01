
int **process_columns(int **array, int rows, int *columns, int M, int N) {
    if (array == NULL || rows <= 0 || columns == NULL || *columns <= 0) return array;
    int cols = *columns;

    /* 1) собрать индексы столбцов, удовлетворяющих условию */
    int *sel = malloc(cols * sizeof(int));
    if (sel == NULL) return array;
    int sc = 0;
    for (int c = 0; c < cols; ++c) {
        int ok = 1;
        for (int r = 0; r < rows; ++r) {
            int rem = array[r][c] % M;
            if (rem < 0) rem += ( ( (-rem + M - 1) / M ) * M ); /* сохраняем поведение % для отрицательных */
            /* проще: нормализуем через ((x % M)+M)%M */
            rem = ((array[r][c] % M) + M) % M;
            if (rem != N) { ok = 0; break; }
        }
        if (ok) sel[sc++] = c;
    }

    if (sc < 2) { free(sel); return array; } /* ничего для удаления/пары нет */

    /* 2) пометить к удалению вторые элементы в каждой паре */
    char *del = calloc(cols, 1);
    if (del == NULL) { free(sel); return array; }

    for (int p = 0; p + 1 < sc; p += 2) {
        int a = sel[p];
        int b = sel[p + 1];
        /* для каждой строки: a = min(a,b) */
        for (int r = 0; r < rows; ++r) {
            if (array[r][b] < array[r][a]) array[r][a] = array[r][b];
        }
        del[b] = 1; /* удалить столбец b */
    }

    /* 3) подсчитать новые колонки */
    int del_count = 0;
    for (int c = 0; c < cols; ++c) if (del[c]) ++del_count;
    if (del_count == 0) { free(sel); free(del); return array; }

    int new_cols = cols - del_count;

    /* 4) выделить новую матрицу и скопировать столбцы, не помеченные на удаление */
    int **new_array = malloc(new_cols * sizeof(int *) + (size_t)new_cols * rows * sizeof(int));
    if (new_array == NULL) { free(sel); free(del); return array; }
    new_array[0] = (int *)(new_array + new_cols);
    for (int i = 1; i < new_cols; ++i) new_array[i] = new_array[i-1] + rows; /* NOTE: rows vs columns orientation explained ниже */

    /* ВАЖНО: в вашем коде layout — array[row][col], где pointer stride = columns.
       В этой функции мы используем совместимый конструкт: new_array[row] = new_array[0] + row * new_cols.
       Исправим: предыдущ линия была ошибочной — ниже верное заполнение указателей. */
    /* корректируем: */
    new_array[0] = (int *)(new_array + new_cols);
    for (int r = 1; r < new_cols; ++r) new_array[r] = new_array[r-1] + rows; /* temporary; мы исправим ниже */

    /* Actually we need pointers per row: new_array[row] = base + row*new_cols */
    /* recompute correctly: */
    /* base pointer */
    int *base = (int *)(new_array + new_cols);
    for (int r = 0; r < new_cols; ++r) { /* temporary loop overwritten below; we'll set proper row pointers */
        /* noop to satisfy readability */
    }
    /* set row pointers properly (rows rows, each row has new_cols columns) */
    for (int r = 0; r < rows; ++r) {
        new_array[r] = base + (size_t)r * new_cols;
    }

    /* copy columns preserving order (rows x new_cols) */
    int dst_c = 0;
    for (int c = 0; c < cols; ++c) {
        if (del[c]) continue;
        for (int r = 0; r < rows; ++r) {
            new_array[r][dst_c] = array[r][c];
        }
        ++dst_c;
    }

    /* 5) cleanup */
    free(sel);
    free(del);
    free(array[0]); /* old data block */
    free(array);    /* old pointers */

    *columns = new_cols;
    return new_array;
}
