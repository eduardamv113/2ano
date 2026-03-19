#include "matrix.h"


int **createMatrix() {

    // seed random numbers
    srand(123456); // fixed seed
    //srand(time(NULL)); // seed based on time - always changing

    // Allocate and populate matrix with random numbers.
    printf("Generating numbers from 0 to %d...", MAX_RAND);
    int **matrix = (int **) malloc(sizeof(int*) * ROWS);
    for (int i = 0; i < ROWS; i++) {
        matrix[i] = (int*) malloc(sizeof(int) * COLUMNS);
        for (int j = 0; j < COLUMNS; j++) {
            matrix[i][j] = rand() % MAX_RAND;
        }
    }
    printf("Done.\n");

    return matrix;
}

void printMatrix(int **matrix) {

    for (int i = 0; i < ROWS; i++) {
        printf("%2d | ", i);
        for (int j = 0; j < COLUMNS; j++) {
            printf("%7d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void lookupNumber(int** matrix, int value, int* vector){
    int pd[ROWS][2];
    pid_t pids[ROWS];

    // Create pipes for each row
    for (int i = 0; i < ROWS; i++) {
        if (pipe(pd[i]) == -1) {
            perror("pipe");
            return;
        }
    }

    // Fork a child process for each row
    for (int i = 0; i < ROWS; i++) {
        pids[i] = fork();
        
        if (pids[i] < 0) {
            perror("fork");
            return;
        }
        
        if (pids[i] == 0) {
            /* CHILD: process row i */
            // Close all write ends (we only need to write to our own pipe)
            for (int j = 0; j < ROWS; j++) {
                if (i != j) {
                    close(pd[j][0]);
                    close(pd[j][1]);
                } else {
                    close(pd[j][0]); // Close read end of our pipe
                }
            }
            
            // Count occurrences of value in row i
            int count = 0;
            for (int j = 0; j < COLUMNS; j++) {
                if (matrix[i][j] == value) {
                    count++;
                }
            }
            
            // Send result through pipe
            Minfo info;
            info.line_nr = i;
            info.ocur_nr = count;
            
            if (write(pd[i][1], &info, sizeof(Minfo)) == -1) {
                perror("write");
                exit(1);
            }
            
            close(pd[i][1]);
            exit(0);
        }
    }

    /* PARENT: collect results */
    // Close all write ends
    for (int i = 0; i < ROWS; i++) {
        close(pd[i][1]);
    }

    // Read from each pipe
    for (int i = 0; i < ROWS; i++) {
        Minfo info;
        
        if (read(pd[i][0], &info, sizeof(Minfo)) == -1) {
            perror("read");
            return;
        }
        
        // Store the count in the vector at position corresponding to line number
        vector[info.line_nr] = info.ocur_nr;
        
        close(pd[i][0]);
    }

    // Wait for all children to finish
    for (int i = 0; i < ROWS; i++) {
        int status;
        if (waitpid(pids[i], &status, 0) == -1) {
            perror("waitpid");
            return;
        }
    }
}