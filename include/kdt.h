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
#define KDT_LEFT(i) (i<<1)
#define KDT_RIGHT(i) ((i<<1)+1)
#define KDT_SWAP(x,y) aux = x; x = y; y = aux

// ---------------
// DECLARATIONS
// ---------------
struct kdt;
struct kdt_mem;
inline float kdt_dist(float pixel, float npixel);
float kdt_update_knn(float *pixels, struct kdt *currn, struct kdt **knn, float *knd, unsigned k);
void kdt_query_knn_r(float* pixels, struct kdt *currn, struct kdt **knn, float *knd, unsigned k);
void kdt_query_knn(float *pixels, struct kdt *root, unsigned k);
void kdt_build(struct kdt_mem *mem, struct df *df);
void kdt_build_r(struct kdt_mem *mem, struct df *df, int start, int end, int curr, int depth, int idx[]);
void kdt_sort(struct df *df, int start, int end, int axis, int idx[]);



// ---------------
// DEFINITIONS
// ---------------

struct kdt{
    float part;
    int dim;
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


void kdt_build(struct kdt_mem *mem, struct df *df){
	int  idx[MAX_CAPACITY], i, df_sz;
		
	mem->size = 0
	i = 0;
	df_sz = df->size;
	for(i = 0; i<df_sz; ++i){
		aux[i] = i;
	}

	kdt_build_r(mem, df, 0, df_sz, 0, 0, idx);
	
}

void kdt_build_r(struct kdt_mem *mem, struct df *df, int start, int end, int curr, int depth, int idx[]){
	int i, j, axis, median;
	
	if(start == end) return;
	axis = depth % PIXELS;
	median = (start+end)/2;
	kdt_sort(df, start, end, axis, inx);
	mem->size += 1;
	i = idx[median];
	mem->mem[curr].part = df->pixels[i][axis];
	mem->mem[curr].dim = axis;
	mem->mem[curr].label = df->labels[i];
	for(j = 0; j<PIXELS; ++j){
		mem->mem[curr].pixels[j] = df->pixels[i][j];
	}
	kdt_build_r(mem, df, start, median, KDT_LEFT(curr), depth+1, idx);
	kdt_build_r(mem, df, median+1, end, KDT_RIGHT(curr), depth+1, idx);
}

void kdt_sort(struct df *df, int start, int end, int axis, int idx[]){
	int aux, i_pivot, i, j;
	float v_pivot;
	if(start == end) return;
	i_pivot = (rand()%(end-start))+start;
	KDT_SWAP(idx[start], idx[i_pivot]);
	v_pivot = df->pixels[idx[start]][axis];
	i = start;
	for(j = i + 1; j<=end; ++j){
		if(df->pixels[idx[j]][axis] < v_pivot){
			i += 1;
			SWAP(idx[i], idx[j]);
		}
	}
	SWAP(idx[start], idx[i]);

	kdt_sort(df, start, i, axis, idx);
	kdt_sort(df, i+1, end, axis, idx);
}

#endif
