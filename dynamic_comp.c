#include "math.h"
#include <emscripten/emscripten.h>


typedef struct {
    double r, g, b;
} Color;

typedef struct {
    double x, y;
    Color color;
} ColorInfo;

double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

// ReLU Function
double relu(double x) {
    return (x > 0) ? x : 0;
}

// Saturation Function
double saturation(double x) {
    if (x < 0) return 0;
    if (x > 1) return 1;
    return x;
}

// Square Wave Function
double squareWave(double x) {
    return ((int)floor(x) % 2 == 0) ? 1 : -1;
}

// Sawtooth Wave Function
double sawtoothWave(double x) {
    return 2 * (x - floor(x + 0.5));
}

// Triangle Wave Function
double triangleWave(double x) {
    return 4 * fabs(x - round(x)) - 1;
}

int numColors = 3;

EMSCRIPTEN_KEEPALIVE
double* interpolateColor(double x, double y, double t) {
   
    double* finalColorArray = (double*)malloc(3 * sizeof(double));
    if (!finalColorArray) {
        // Handle memory allocation failure
        return NULL;
    }

    ColorInfo colors[] = {
        //{ 100 + cos(t)*100, -400 + sin(t)*100, {232, 112, 109} },
        //{ 200 - cos(t)*100, 50 + cos(t)*100, {112, 98, 232} },
        { -250 + cos(t)*100, -450 + sin(t)*100, {206, 98, 212} },
        { 440 - pow(cos(t), 2)*100, 460 - cos(t)*100, {255, 255, 255} },
        { -300 - pow(cos(t), 2)*100, -460 - cos(t)*100, {65, 18, 255} }
    };

    finalColorArray[0] = 0;
    finalColorArray[1] = 0;
    finalColorArray[2] = 0;
    
    double totalWeight = 0;

    for (int i = 0; i < numColors; i++) {
        ColorInfo colorInfo = colors[i];
        double dx = colorInfo.x - x;
        double dy = colorInfo.y - y;
        double distance = sqrt(dx * dx + dy * dy);

        double weight = sinh(distance/x)/distance;
        totalWeight += weight;

        finalColorArray[0] += colorInfo.color.r * weight;
        finalColorArray[1] += colorInfo.color.g * weight;
        finalColorArray[2] += colorInfo.color.b * weight;
    }

    finalColorArray[0] /= totalWeight;
    finalColorArray[1] /= totalWeight;
    finalColorArray[2] /= totalWeight;

    
    return finalColorArray;
}

EMSCRIPTEN_KEEPALIVE
void free_memory(double* ptr) {
    free(ptr);
}

