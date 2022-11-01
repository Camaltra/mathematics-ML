#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct CSR {
    size_t n_rows;
    size_t n_cols;
    size_t n_nzero;
    size_t* row_ptrs;
    size_t* cols_idx;
    size_t* value;
} CSR;

size_t get_n_nzero(double *A, size_t n_rows, size_t n_cols);
int create_spare_matrix(double *A, size_t n_rows, size_t n_cols, size_t n_nzero, CSR *A_csr);
int free_spare_matrix(CSR *A_csr);
int print_sparse_matrix(const CSR A_csr);

/**
 * main - Exemple of the sparse matrix
 * Return: 1 if success
*/
int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
    size_t n_rows = 5;
    size_t n_cols = 5;
    size_t n_nzero;
    size_t A_size = n_cols * n_rows;
    CSR A_csr;

    double A[] = {
        1, 0, 0, 2, 0,
        0, 5, 3, 0, 0,
        2, 0, 0, 0, 0,
        0, 0, 0, 19, 0,
        0, 0, 8, 0, 0
    };

    n_nzero = get_n_nzero(A, n_rows, n_cols);

    if (A_size - n_nzero < A_size / 2) {
        printf("This don't deserve to be transform to a sparse matrix, no gain of space, skipped...");
        return EXIT_SUCCESS;
    }

    create_spare_matrix(A, n_rows, n_cols, n_nzero, &A_csr);
    print_sparse_matrix(A_csr);
    free_spare_matrix(&A_csr);
    return EXIT_SUCCESS;
}


/**
* get_n_nzero - Get the number of non-zero element
* @A: The matrix to process
* Return: The numeber of non-zero element
*/
size_t get_n_nzero(double *A, size_t n_rows, size_t n_cols) {
    size_t i;
    size_t n_nzero = 0;
    size_t A_size = n_rows * n_cols;

    for (i = 0; i < A_size; i++) {
        if (A[i] != 0) {
            ++n_nzero;
        }
    }
    return n_nzero;
}

/**
 * create_spare_matrix - Create a sparse matrix like CSR
 * @A: The matrix to transform
 * @n_row: The number of row
 * @n_cols: The number of cols of the matrix A
 * @n_nzero: The number of non-zero num of the matrix A
 * @A_csr: Pointer to the CSR matrix
 * Return: 1 For success
*/
int create_spare_matrix(double *A, size_t n_rows, size_t n_cols, size_t n_nzero, CSR *A_csr) {
    size_t i, j;
    size_t nzero_id = 0;

    A_csr->n_rows = n_rows;
    A_csr->n_cols = n_cols;
    A_csr->n_nzero = n_nzero;
    A_csr->row_ptrs = calloc(n_rows + 1, sizeof(size_t));
    A_csr->cols_idx = calloc(n_nzero, sizeof(size_t));
    A_csr->value = calloc(n_nzero, sizeof(double));

    for (i = 0; i < A_csr->n_rows; i++) {
        A_csr->row_ptrs[i] = nzero_id;
        for(j = 0; j < A_csr->n_cols; j++) {
            if (A[i * A_csr->n_cols + j] != 0.0) {
                A_csr->cols_idx[nzero_id] = j;
                A_csr->value[nzero_id] = A[i * A_csr->n_cols + j];
                ++nzero_id;
            }
        }
    }
    A_csr->row_ptrs[A_csr->n_rows] = nzero_id;
    return EXIT_SUCCESS;
}

/**
 * free_spare_matrix - Free the sparse matrix
 * @A_csr: The sparse matrix to free
 * Return: 1 For success
*/
int free_spare_matrix(CSR *A_csr) {
    free(A_csr->row_ptrs);
    free(A_csr->cols_idx);
    free(A_csr->value);
    return EXIT_SUCCESS;
}

/**
 * print_sparse_matrix - Print the sparse matrix
 * @A_csr: The matrix to print
 * Return: 1 if success
*/
int print_sparse_matrix(const CSR A_csr) {
    size_t i;
    size_t j;
    double value;

    printf("row\t|\tcolums\t|\tvalue\n");
    printf("-------------------------------------\n");
    for (i = 0; i < A_csr.n_rows; i++) {
        size_t nzero_start = A_csr.row_ptrs[i];
        size_t nzero_end = A_csr.row_ptrs[i + 1];
        for (; nzero_start < nzero_end; nzero_start++) {
            j = A_csr.cols_idx[nzero_start];
            value = A_csr.value[nzero_start];
            printf("%ld\t|\t%ld\t|\t%2.f\n", i, j, value);
        }
    }
    return EXIT_SUCCESS;
}
