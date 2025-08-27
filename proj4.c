#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include "proj4.h"

// Struct to hold arguments for thread functions
typedef struct {
  grid *input;         // Pointer to the input grid
  unsigned long s;     // Target sum for diagonals
  grid *output;        // Pointer to the output grid
  int start_row;       // Starting row for processing
  int end_row;         // Ending row for processing
} ThreadArgs;

// Function executed by threads to compute diagonal sums
void *computeDiagonalSums(void *arg) {
  ThreadArgs *args = (ThreadArgs *)arg;
  grid *input = args->input;
  unsigned long s = args->s;
  grid *output = args->output;
  int start_row = args->start_row;
  int end_row = args->end_row;

  // Iterate through rows assigned to this thread
  for (int i = start_row; i < end_row; i++) {
    for (int j = 0; j < input->n; j++) {
      // Compute right diagonal sums starting at (i, j)
      unsigned long sum = 0;
      for (int k = 0; k < input->n - j && i + k < input->n; k++) {
        sum += input->p[i + k][j + k] - '0'; // Convert character to digit and add
        if (sum == s) { // If the target sum is found
          for (int m = 0; m <= k; m++) {
            output->p[i + m][j + m] = input->p[i + m][j + m]; // Copy diagonal to output
          }
        }
        if (sum > s) { // Exit loop early if sum exceeds target
          k = input->n; 
        }
      }

      // Compute left diagonal sums starting at (i, j)
      sum = 0;
      for (int k = 0; k < input->n && i + k < input->n && j - k >= 0; k++) {
        sum += input->p[i + k][j - k] - '0'; // Convert character to digit and add
        if (sum == s) { // If the target sum is found
          for (int m = 0; m <= k; m++) {
            output->p[i + m][j - m] = input->p[i + m][j - m]; // Copy diagonal to output
          }
        }
        if (sum > s) { // Exit loop early if sum exceeds target
          k = input->n;
        }
      }
    }
  }
  return NULL; // Return from thread function
}

// Function to initialize a grid from a file
void initializeGrid(grid *g, char *fileName) {
  FILE *file = fopen(fileName, "r"); // Open file for reading
  if (file == NULL) { // Check if file was opened successfully
    perror("Error opening file");
    exit(1);
  }

  char line[10000];  // Buffer to read each line from the file
  if (fgets(line, sizeof(line), file) == NULL) { // Read the first line
    fprintf(stderr, "Error reading input\n");
    exit(1);
  }

  g->n = strlen(line) - 1;  // Determine grid size (subtract 1 for newline)
  g->p = (unsigned char **)malloc(g->n * sizeof(unsigned char *)); // Allocate rows
    
  for (unsigned int i = 0; i < g->n; i++) {
    g->p[i] = (unsigned char *)malloc(g->n * sizeof(unsigned char)); // Allocate columns
    if (i == 0) {
      strncpy((char *)g->p[i], line, g->n); // Copy the first line into grid
    } else {
      if (fgets((char *)g->p[i], g->n + 2, file) == NULL) {  // Read subsequent lines
        fprintf(stderr, "Error reading line %u\n", i);
        exit(1);
      }
    }
  }

  fclose(file); // Close the file
}

// Function to compute diagonal sums using threads
void diagonalSums(grid *input, unsigned long s, grid *output, int t) {
  // Initialize output grid
  output->n = input->n;
  output->p = (unsigned char **)malloc(output->n * sizeof(unsigned char *));
  for (unsigned int i = 0; i < output->n; i++) {
    output->p[i] = (unsigned char *)calloc(output->n, sizeof(unsigned char)); // Initialize to 0
  }

  if (t == 1) { // Single-threaded execution
    ThreadArgs args = {input, s, output, 0, input->n};
    computeDiagonalSums(&args);
  } else { // Multi-threaded execution
    pthread_t threads[3];
    ThreadArgs args[3];
    int rows_per_thread = input->n / t; // Determine rows per thread

    for (int i = 0; i < t; i++) {
      args[i].input = input;
      args[i].s = s;
      args[i].output = output;
      args[i].start_row = i * rows_per_thread;
      args[i].end_row = (i == t - 1) ? input->n : (i + 1) * rows_per_thread;
      pthread_create(&threads[i], NULL, computeDiagonalSums, &args[i]); // Create thread
    }

    for (int i = 0; i < t; i++) {
      pthread_join(threads[i], NULL); // Wait for threads to finish
    }
  }
}

// Function to write grid data to a file
void writeGrid(grid *g, char *fileName) {
  FILE *file = fopen(fileName, "w"); // Open file for writing
  if (file == NULL) {
    perror("Error opening file for writing");
    exit(1);
  }

  for (unsigned int i = 0; i < g->n; i++) {
    for (unsigned int j = 0; j < g->n; j++) {
      if (g->p[i][j] == '\0') {
        fprintf(file, "0"); // Write 0 for empty cells
      } else {
        fprintf(file, "%c", g->p[i][j]); // Write character from grid
      }
    }
    fprintf(file, "\n"); // Add newline after each row
  }

  fclose(file); // Close file
  chmod(fileName, S_IRUSR | S_IWUSR); // Set file permissions
}

// Function to free allocated memory for a grid
void freeGrid(grid *g) {
  for (unsigned int i = 0; i < g->n; i++) {
    free(g->p[i]); // Free each row
  }
  free(g->p); // Free the row pointers
}
