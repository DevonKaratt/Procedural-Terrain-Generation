#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

// Calculate size from n: 2^n + 1
int calculateSize(int n) {
    return (1 << n) + 1;  // 2^n + 1
}

float randomDisplacement(float scale) {
    return (((float)rand() / RAND_MAX) * 2 - 1) * scale;
}

float clamp(float val) {
    if (val < 0) return 0;
    if (val > 1) return 1;
    return val;
}

// Simplified and corrected diamond-square algorithm
void diamondSquare(float **terrain, int size, float roughness) {
    printf("Starting diamond-square algorithm...\n");
    
    // Initialize corners
    terrain[0][0] = 0.5f;
    terrain[0][size-1] = 0.5f;
    terrain[size-1][0] = 0.5f;
    terrain[size-1][size-1] = 0.5f;

    int step = size - 1;
    float scale = 1.0f;
    int iteration = 0;

    while (step > 1) {
        printf("Iteration %d: step=%d, scale=%.3f\n", iteration, step, scale);
        int half = step / 2;

        // Diamond step: set center of each square
        for (int y = 0; y < size - 1; y += step) {
            for (int x = 0; x < size - 1; x += step) {
                if (x + half < size && y + half < size) {
                    float avg = (terrain[x][y] + 
                                terrain[x + step][y] + 
                                terrain[x][y + step] + 
                                terrain[x + step][y + step]) / 4.0f;
                    terrain[x + half][y + half] = clamp(avg + randomDisplacement(scale));
                }
            }
        }

        // Square step: set midpoints of each diamond
        for (int y = 0; y < size; y += half) {
            for (int x = (y + half) % step; x < size; x += step) {
                if (x < size && y < size) {
                    float sum = 0.0f;
                    int count = 0;

                    // Average the four diamond points
                    if (x >= half) { 
                        sum += terrain[x - half][y]; 
                        count++; 
                    }
                    if (x + half < size) { 
                        sum += terrain[x + half][y]; 
                        count++; 
                    }
                    if (y >= half) { 
                        sum += terrain[x][y - half]; 
                        count++; 
                    }
                    if (y + half < size) { 
                        sum += terrain[x][y + half]; 
                        count++; 
                    }

                    if (count > 0) {
                        // Only set if not already set (to avoid overwriting diamond points)
                        if (terrain[x][y] == 0) {
                            terrain[x][y] = clamp((sum / count) + randomDisplacement(scale));
                        }
                    }
                }
            }
        }

        step = step/2;
        scale *= roughness;
        iteration++;
        
        // Safety break for debugging
        if (iteration > 10) {
            printf("Safety break after 10 iterations\n");
            break;
        }
    }
    printf("Diamond-square algorithm completed\n");
}

// Print terrain as numeric values to terminal (0-255)
void printNumericTerrain(float **terrain, int size) {
    printf("\n=== GENERATED TERRAIN (NUMERIC VALUES 0-255) ===\n\n");
    
    // Print column indices
    printf("     ");
    for (int x = 0; x < size; x++) {
        printf("%3d ", x);
    }
    printf("\n");
    
    for (int y = 0; y < size; y++) {
        printf("%2d: ", y);
        for (int x = 0; x < size; x++) {
            // Scale to 0-255
            printf("%3d ", (int)(terrain[x][y] * 255));
        }
        printf("\n");
    }
    printf("\n");
}

void saveAsPGM(float **terrain, int size, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Could not create file %s\n", filename);
        return;
    }
    
    fprintf(file, "P2\n");
    fprintf(file, "# Diamond-Square terrain\n");
    fprintf(file, "%d %d\n", size, size);
    fprintf(file, "255\n");
    
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            int value = (int)(terrain[x][y] * 255);
            fprintf(file, "%d ", value);
        }
        fprintf(file, "\n");
    }
    
    fclose(file);
    printf("Terrain saved as %s\n", filename);
}

int main() {
    srand(time(NULL));

    int n, size;
    char filename[100];

    printf("Enter the value of n (size will be 2^n + 1): ");
    printf("\nRecommended values: n=3 (9x9), n=4 (17x17), n=5 (33x33)\n");
    printf("Enter n: ");
    scanf("%d", &n);

    // Validate n to prevent large allocations during testing
    if (n < 1 || n > 8) {
        printf("Error: n must be between 1 and 8\n");
        return 1;
    }

    size = calculateSize(n);
    printf("Generating terrain of size: %dx%d (2^%d + 1)\n", size, size, n);

    printf("Enter output filename (without extension): ");
    scanf("%s", filename);
    strcat(filename, ".pgm");

    printf("Allocating memory for %dx%d terrain...\n", size, size);
    
    // Allocate terrain
    float **terrain = (float **)malloc(size * sizeof(float *));
    if (terrain == NULL) {
        printf("Error: Memory allocation failed!\n");
        return 1;
    }
    
    for (int i = 0; i < size; i++) {
        terrain[i] = (float *)malloc(size * sizeof(float));
        if (terrain[i] == NULL) {
            printf("Error: Memory allocation failed for row %d!\n", i);
            return 1;
        }
        // Initialize to 0
        for (int j = 0; j < size; j++) {
            terrain[i][j] = 0.0f;
        }
    }

    printf("Generating terrain using diamond-square algorithm...\n");
    diamondSquare(terrain, size, 0.5f);

    // Print to terminal first (0-255 values)
    printf("\n============================================\n");
    printNumericTerrain(terrain, size);

    printf("Saving to file...\n");
    saveAsPGM(terrain, size, filename);

    // Free memory
    for (int i = 0; i < size; i++) {
        free(terrain[i]);
    }
    free(terrain);

    printf("Done! Check the file: %s\n", filename);
    return 0;
}