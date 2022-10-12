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
	int label;
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
	int i, j, *labels, fr[LABELS], div[LABELS][2], uniques, sz, div_sz[2], best_pixel_split, aux;
	float avgs[PIXELS], (*pixel)[PIXELS], gini_index[2], gini_split, best_gini_split, prob[2];
	
	if(start >= end){
		mem->mem[curr].label = -1;
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
		mem->mem[curr].label = -1;
		for(i = 0; i<LABELS; ++i){
			if(fr[i]){
				mem->mem[curr].label = i;
				break;
			} 
		}
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
	
	// Para cada media (pixel)
		// Dividir segun si son mayores o no
		// Calcular  gini de la división
		// Guardar si es el mejor
	best_pixel_split = 0;
	best_gini_split = 1000;
	for(i = 0; i<PIXELS; ++i){
	
		for(j = 0; j<LABELS; ++j){
			div[j][0] = 0;
			div[j][1] = 0;
		}
		
		for(j = start; j<end; ++j){
			pixel = df->pixels[idx[j]];
			div[labels[j]][0] += (*pixel[i] < avgs[i])*1;
			div[labels[j]][1] += (*pixel[i] >= avgs[i])*1;
		}

		gini_index[0] = 0;
		gini_index[1] = 0;
		div_sz[0] = 0;
		div_sz[1] = 0;
		
		for(j = 0; j<LABELS; ++j){
			div_sz[0] += div[j][0];
			div_sz[1] += div[j][1];
		
			prob[0] = div[j][0]/sz;
			prob[1] = div[j][1]/sz;
			
			gini_index[0] += prob[0]*prob[0];
			gini_index[1] += prob[1]*prob[1]; 			 
		}
		
		gini_index[0] = 1 - gini_index[0];
		gini_index[1] = 1 - gini_index[1];

		gini_split = (div_sz[0]/sz)*gini_index[0] + (div_sz[1]/sz)*gini_index[1];

		if(gini_split < best_gini_split){
			best_pixel_split = i;
			best_gini_split = gini_split;
		}
	}

	mem->size += 1;
	mem->mem[curr].label = -2;
	mem->mem[curr].dim = best_pixel_split;
	mem->mem[curr].part = avgs[best_pixel_split];

	
	// Dividir segun esa media
	i = start;
	for(j = i + 1; j<end; ++j){
		pixel = df->pixels[idx[j]];
		if(*pixel[best_pixel_split] < avgs[best_pixel_split]){
			i += 1;
			aux = idx[i];
			idx[i] = idx[j];
			idx[j] = aux;
		}
	}
	aux = idx[i];
	idx[i] = idx[start];
	idx[start] = aux;
	i += (df->pixels[idx[i]][best_pixel_split] < avgs[best_pixel_split]);
	
	
	// Llamar recursivamente para cada una de las dos partes
	dt_build_r(mem, df, DT_LEFT(curr), start, i, idx);
	dt_build_r(mem, df, DT_RIGHT(curr), i, end, idx);
}



#endif
