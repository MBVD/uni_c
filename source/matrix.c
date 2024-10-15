#include "../include/matrix.h"
#include <time.h>
matrix crete_matrix(int m, int n){
  int **ptr = (int**)calloc(m, sizeof(int*));
  for (int i = 0; i<m; i++){
    ptr[i] = (int*)calloc(n, sizeof(int));
  }
  matrix x = {.m = m, .n = n, .ptr = ptr};
  return x;
}

void delete_matrix(matrix x){
  for (int i = 0; i<x.m; i++){
    free(x.ptr[i]);
  }
}

void randomize(matrix x){
  srand(time(NULL));
  for (int i = 0; i<x.m; i++){
    for (int j = 0; j<x.n; j++){
      x.ptr[i][j] = rand();
    }
  }
}

void print_matrix(matrix x){
  for (int i = 0; i<x.m; i++){
    for (int j =0; j<x.n; j++){
      printf("%d ", x.ptr[i][j]);
    }
  }
}