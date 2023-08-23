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

int numColors = 5;

EMSCRIPTEN_KEEPALIVE
double* interpolateColor(double x, double y, double t) {
    y = y/4;
    double* finalColorArray = (double*)malloc(3 * sizeof(double));
    if (!finalColorArray) {
        // Handle memory allocation failure
        return NULL;
    }

    ColorInfo colors[] = {
        { 100 + cos(t)*100, -400 + sin(t)*100, {232, 112, 109} },
        { 200 - cos(t)*100, 50 + cos(t)*100, {112, 98, 232} },
        { -250 + cos(t)*100, -450 + sin(t)*100, {206, 98, 212} },
        { 440 - pow(cos(t), 2)*100, 460 - cos(t)*100, {0, 22, 212} },
        { -300 - pow(cos(t), 2)*100, -460 - cos(t)*100, {255, 255, 255} }
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

        double weight = sigmoid(distance/y);
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

