typedef struct matrix{
  int m, n, **ptr;
} matrix;
matrix create_matrix(int, int);
void delete_matrix(matrix);
void randomize(matrix);
void print_matrix(matrix);
matrix copy_matrix(matrix);