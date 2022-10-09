#ifndef __KDT_H__
#define __KDT_H__


// ---------------
// INCLUDES
// ---------------
#include <stdlib.h>
#include "df.h"



// ---------------
// CONSTANTS
// ---------------
#define KDT_MEM (1024*32)


// ---------------
// DECLARATIONS
// ---------------
struct kdt;
struct kdt_mem;
inline float kdt_dist(float pixel, float npixel);
float kdt_update_knn(float *pixels, struct kdt *currn, struct kdt **knn, float *knd, unsigned k);
void kdt_query_knn_r(float* pixels, struct kdt *currn, struct kdt **knn, float *knd, unsigned k);
void kdt_query_knn(float *pixels, struct kdt *root, unsigned k);
void kdt_build(float (*pixels)[PIXELS], int size, int depth);

// ---------------
// DEFINITIONS
// ---------------

struct kdt{
    float part;
    int dim;
    struct kdt *left;
    struct kdt *right;
    int label;
    float pixels[PIXELS];
};

struct kdt_mem{
	int size;
	struct kdt mem[KDT_MEM];	
};

inline float kdt_dist(float pixel, float npixel){
    float diff = pixel - npixel;
    return diff * diff;
}

float kdt_update_knn(float *pixels, struct kdt *currn, struct kdt **knn, float *knd, unsigned k){
    int i, j, worst_i;
    float worst, currd, *npixels;

    npixels = currn->pixels;
    for(i = 0; i<k; ++i){
        if(knd[i] > knd[worst_i]) {
            worst_i = i;
        }
    }
    worst = knd[worst_i];

    for(i = 0; i<PIXELS; ++i){
        currd += kdt_dist(pixels[i], npixels[i]);
    }
    if(currd > worst){
        knn[worst_i] = currn;
        knd[worst_i] = currd;
    }

    return worst;
}


void kdt_query_knn_r(float* pixels, struct kdt *currn, struct kdt **knn, float *knd, unsigned k){
    if(currn == NULL) return;

    if(kdt_dist(pixels[currn->dim], currn->part) > kdt_update_knn(pixels, currn, knn, knd, k)) return;

    kdt_query_knn_r(pixels, currn->left, knn, knd, k);
    kdt_query_knn_r(pixels, currn->right, knn, knd, k);
}

void kdt_query_knn(float *pixels, struct kdt *root, struct kdt *knn[], unsigned k){
    // Crear algo para almacenar los K vecinos mas cercanos hasta el momento
    // Inicializar con K INFs
    // Partes por la raiz
    // Si la distancia a la partición actual es menor que algunas de los k vecinos
        // Guardar la distancia al nodo y repetir para sus hijos
    int i;
    float knd[k];

    for(i = 0; i<k; i++) knd[i] = 100000;

    kdt_query_knn_r(pixels, root, knn, knd, k);
}


#endif
