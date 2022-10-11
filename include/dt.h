#ifndef __DT_H__
#define __DT_H__


// ---------------
// INCLUDES
// ---------------
#include <stdlib.h>
#include "df.h"




// ---------------
// CONSTANTS
// ---------------
#define DT_MEM (1024*64)
#define DT_LEFT(i) (i<<1)
#define DT_RIGHT(i) ((i<<1)+1)




// ---------------
// DECLARATIONS
// ---------------
struct dt;
struct dt_mem;
void dt_build(struct dt_mem *mem, struct df *df);
void dt_build_r(struct dt_mem *mem, struct df *df, int curr, int start, int end, int *idx);




// ---------------
// DEFINITIONS
// ---------------
struct dt{
	int dim;
	float part;
};

struct dt_mem{
	int size;
	struct dt mem[DT_MEM];
};

int _idx[MAX_CAPACITY];
void dt_build(struct dt_mem *mem, struct df *df){
	int i;
	
	mem->size = 0;
	for(i = 0; i<mem->size; ++i){
		_idx[i] = i;
	}
	
	dt_build_r(mem, df, 1, 0, df->size, _idx);
}

void dt_build_r(struct dt_mem *mem, struct df *df, int curr, int start, int end, int *idx){
	int i, j, *labels, fr[LABELS], uniques, sz;
	float avgs[PIXELS], (*pixel)[PIXELS];
	
	if(start >= end){
		return;
	}

	sz = end - start;
	
	// Contar la cantidad de labels diferentes
	labels = df->label;
	for(i = 0; i<LABELS; ++i){
		fr[i] = 0;
	}
	for(i = start; i<end; ++i){
		fr[labels[idx[i]]] += 1;
	}
	uniques = 0;
	for(i = 0; i<LABELS; ++i){
		uniques += ( fr[i] > 0 );
	}
	
	// Si solo hay un label, devolver
	if(uniques < 2){
		return;
	}
	
	// Calcular la media de cada pixel
	for(i = 0; i<PIXELS; ++i){
		avgs[i] = 0;
	}
	for(i = start; i<end; ++i){
		pixel = df->pixels[idx[i]];
		for(j = 0; j<PIXELS; ++j){
			avgs[j] += (*pixel)[j];
		}
	}
	for(i = 0; i<PIXELS; ++i){
		avgs[i] = avgs[i] / sz;
	}
	
	// Para cada media
		// Dividir segun si son mayores o no
		// Calcular y guardar el gini de la división
	// Seleccionar la media con mejor gini
	// Dividir segun esa media
	// Llamar recursivamente para cada una de las dos partes
}



#endif
