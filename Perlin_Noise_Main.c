#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef struct {
    float x, y;
} vector2;

vector2 randomGradient(int ix, int iy) {
    unsigned int w = 8 * sizeof(unsigned int);
    unsigned int s = w / 2;
    unsigned int a = ix, b = iy;
    a *= 3284157443U;
    b ^= (a << s) | (a >> (w - s));
    b *= 1911520717U;
    a ^= (b << s) | (b >> (w - s));
    a *= 2048419325U;
    float random = a * (3.14159265f / (~(~0u >> 1)));
    vector2 v;
    v.x = sinf(random);
    v.y = cosf(random);
    return v;
}

float dotGridGradient(int ix, int iy, float x, float y) {
    vector2 gradient = randomGradient(ix, iy);
    float dx = x - (float)ix;
    float dy = y - (float)iy;
    return (dx * gradient.x + dy * gradient.y);
}

float interpolate(float a0, float a1, float w) {
    return (a1 - a0) * (3.0f - 2.0f * w) * w * w + a0;
}

float perlin(float x, float y) {
    int x0 = (int)x;
    int x1 = x0 + 1;
    int y0 = (int)y;
    int y1 = y0 + 1;

    float sx = x - (float)x0;
    float sy = y - (float)y0;

    float n0 = dotGridGradient(x0, y0, x, y);
    float n1 = dotGridGradient(x1, y0, x, y);
    float ix0 = interpolate(n0, n1, sx);

    n0 = dotGridGradient(x0, y1, x, y);
    n1 = dotGridGradient(x1, y1, x, y);
    float ix1 = interpolate(n0, n1, sx);

    return interpolate(ix0, ix1, sy);
}

int main() {
    int width, height, GRID_SIZE;
    printf("Enter image width: ");
    scanf("%d", &width);
    printf("Enter image height: ");
    scanf("%d", &height);
    printf("Enter grid size (e.g., 100): ");
    scanf("%d", &GRID_SIZE);

    int maxValue = 255; // 8-bit

    srand((unsigned int)time(NULL));
    float seedX = rand() % 10000;
    float seedY = rand() % 10000;

    FILE *image = fopen("perlin_debug.pgm", "wb");
    if (!image) {
        printf("❌ Could not create file.\n");
        return 1;
    }

    fprintf(image, "P5\n%d %d\n%d\n", width, height, maxValue);

    int octaveCount = 6;
    float persistence = 0.5f, lacunarity = 2.0f;

    float* vals = malloc(width * height * sizeof(float));
    float min = 9999, max = -9999;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float fx = (x + seedX) / GRID_SIZE;
            float fy = (y + seedY) / GRID_SIZE;
            float value = 0, amp = 1, freq = 1, norm = 0;

            for(int o = 0; o < octaveCount; o++) {
                value += perlin(fx * freq, fy * freq) * amp;
                norm += amp;
                freq *= lacunarity;
                amp *= persistence;
            }
            value /= norm;
            vals[y * width + x] = value;
            if (value < min) min = value;
            if (value > max) max = value;
        }
    }

    // Output normalized values, both to file and console
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++) {
            float v = vals[y * width + x];
            v = (v - min) / (max - min);
            v = powf(v, 0.8f);
            int mapped = (int)(v * maxValue + 0.5f);
            if (mapped < 0) mapped = 0;
            if (mapped > maxValue) mapped = maxValue;
            // Print number to console
            printf("%3d ", mapped);
            fputc(mapped, image);
        }
        printf("\n"); // New row on console output
    }

    fclose(image);
    free(vals);
    printf("\n✅ Perlin heightmap saved to perlin_debug.pgm (and printed above)\n");
    return 0;
}
