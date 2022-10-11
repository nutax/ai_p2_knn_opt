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
#define KDT_MEM (1024*64)
#define KDT_LEFT(i) (i<<1)
#define KDT_RIGHT(i) ((i<<1)+1)
#define KDT_SWAP(x,y) aux = x; x = y; y = aux

// ---------------
// DECLARATIONS
// ---------------
struct kdt;
struct kdt_mem;
float kdt_query_knn_r(struct kdt_mem *mem, float* pixels, int curr , struct kdt **nn, float nd);
void kdt_query_knn(struct kdt_mem *mem, float *pixels, struct kdt **nn);
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

float kdt_query_knn_r(struct kdt_mem *mem, float* pixels, int curr , struct kdt **nn, float nd){
	int i;
	float diff, currd, partd;
	struct kdt *currn;

	currn = &(mem->mem[curr]);

	partd = pixels[currn->dim] - currn->part;
	partd = partd * partd;

	//printf("Axis: %d | Part: %f | Partd: %f | nd: %f\n", currn->dim, currn->part, partd, nd);
	
    if(partd > nd) return nd;

    currd = 0;
    for(i = 0; i<PIXELS; ++i){
    	diff = pixels[i] - (currn->pixels[i]);
    	currd += diff * diff;
    }
    if(currd < nd){
    	//printf("Updated nd from %f to %f\n", nd, currd);
    	nd = currd;
    	*nn = currn;
    }
    nd = kdt_query_knn_r(mem, pixels, KDT_LEFT(curr), nn, nd);
    nd = kdt_query_knn_r(mem, pixels, KDT_RIGHT(curr), nn, nd);
	return nd;
}

void kdt_query_knn(struct kdt_mem *mem, float *pixels, struct kdt **nn){
	float nd;

	nd = 100000;
    nd = kdt_query_knn_r(mem, pixels, 1, nn, nd);
}

int _idx[MAX_CAPACITY];
void kdt_build(struct kdt_mem *mem, struct df *df){
	int i, df_sz;
		
	mem->size = 0;
	i = 0;
	df_sz = df->size;
	for(i = 0; i<df_sz; ++i){
		_idx[i] = i;
	}

	kdt_build_r(mem, df, 0, df_sz, 1, 0, _idx);
	
}

void kdt_build_r(struct kdt_mem *mem, struct df *df, int start, int end, int curr, int depth, int idx[]){
	int i, j, axis, median;
	struct kdt *currn;
	
	if(start >= end){
		mem->mem[curr].part = 10000;
		mem->mem[curr].dim = 0;
		return;	
	}
	axis = depth % PIXELS;
	median = (start+end)/2;
	kdt_sort(df, start, end-1, axis, idx);
	mem->size += 1;
	i = idx[median];
	mem->mem[curr].part = df->pixels[i][axis];
	mem->mem[curr].dim = axis;
	mem->mem[curr].label = df->label[i];
	currn = &(mem->mem[curr]);
	//printf("Curr: %d | Axis: %d | Part: %f | Label: %d\n", curr, currn->dim, currn->part, currn->label);
	
	for(j = 0; j<PIXELS; ++j){
		mem->mem[curr].pixels[j] = df->pixels[i][j];
	}
	kdt_build_r(mem, df, start, median, KDT_LEFT(curr), depth+1, idx);
	kdt_build_r(mem, df, median+1, end, KDT_RIGHT(curr), depth+1, idx);
}

void kdt_sort(struct df *df, int start, int end, int axis, int idx[]){
	int aux, i_pivot, i, j;
	float v_pivot;
	if(start >= end) return;
	i_pivot = (rand()%(end-start))+start;
	KDT_SWAP(idx[start], idx[i_pivot]);
	v_pivot = df->pixels[idx[start]][axis];
	i = start;
	for(j = i + 1; j<=end; ++j){
		if(df->pixels[idx[j]][axis] < v_pivot){
			i += 1;
			KDT_SWAP(idx[i], idx[j]);
		}
	}
	KDT_SWAP(idx[start], idx[i]);

	kdt_sort(df, start, i, axis, idx);
	kdt_sort(df, i+1, end, axis, idx);
}

#endif
