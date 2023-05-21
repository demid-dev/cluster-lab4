#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define VECTOR_SIZE 6

int main(int argc, char** argv) {
    int rank, size;
    int* vector1;
    int* vector2;
    int* local_vector1;
    int* local_vector2;
    int local_size;
    int local_scalar_product;
    int* all_scalar_products;
    int global_scalar_product;
    int i;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        // Read vectors from file (assuming one vector per line)
        FILE* file = fopen("vectors.txt", "r");
        vector1 = (int*)malloc(VECTOR_SIZE * sizeof(int));
        vector2 = (int*)malloc(VECTOR_SIZE * sizeof(int));

        for (i = 0; i < VECTOR_SIZE; i++) {
            fscanf(file, "%d", &vector1[i]);
        }
        for (i = 0; i < VECTOR_SIZE; i++) {
            fscanf(file, "%d", &vector2[i]);
        }

        fclose(file);
    }

    int vector_size = VECTOR_SIZE; // Create a separate variable

    // Broadcast vector size to all processes
    MPI_Bcast(&vector_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Calculate local size
    local_size = vector_size / size;

    // Allocate memory for local vectors
    local_vector1 = (int*)malloc(local_size * sizeof(int));
    local_vector2 = (int*)malloc(local_size * sizeof(int));

    // Scatter vector data to all processes
    MPI_Scatter(vector1, local_size, MPI_INT, local_vector1, local_size, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(vector2, local_size, MPI_INT, local_vector2, local_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Calculate local scalar product
    local_scalar_product = 0;
    for (i = 0; i < local_size; i++) {
        local_scalar_product += local_vector1[i] * local_vector2[i];
    }

    // Gather local scalar products from all processes
    all_scalar_products = (int*)malloc(size * sizeof(int));
    MPI_Allgather(&local_scalar_product, 1, MPI_INT, all_scalar_products, 1, MPI_INT, MPI_COMM_WORLD);

    // Sum all scalar products at the root process (rank 0)
    if (rank == 0) {
        global_scalar_product = 0;
        for (i = 0; i < size; i++) {
            global_scalar_product += all_scalar_products[i];
        }

        printf("Global Scalar Product: %d\n", global_scalar_product);

        free(vector1);
        free(vector2);
    }

    free(local_vector1);
    free(local_vector2);
    free(all_scalar_products);

    MPI_Finalize();

    return 0;
}
