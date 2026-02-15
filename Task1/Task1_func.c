int write_into_array( f, int ** array, int rows, int columns ){
    int current;
    if((rows<=0) || (columns<=0)){
        printf("Invalid input for rows and columns numbers");
        return -11;
    }
    array=(int **)malloc(rows*sizeof(int *) + (rows)*(columns)*sizeof(int));
    array[0] = (int *)(array + N);
	for (i=1; i < rows; i++) {array[i] = array[i-1] + columns;}
    for (j=0; j < columns; j++){
        for (i=0; i < rows; i++){
            if(fscanf(f, "%d", &current)!=1){
               printf("Not enough numbers in the array!");
               return -12;
            }
            array[i][j]=current;
        }
    }
    return 0;
}

int find_needed_string(int **array){
    for (j=0; j < columns; j++){
        for (i=0; i < rows; i++){
            
        }
    }
    
}
