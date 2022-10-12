// ---------------
// INCLUDES
// ---------------
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdalign.h>
#include <time.h>
#include "df.h" // Data info
#include "dt.h"




// ---------------
// CONSTANTS
// ---------------




// ---------------
// DECLARATIONS
// ---------------
int main(int argc, char **argv);
void read_dtree(const char *path, struct dt_mem *mem);
void predict(struct dt_mem *dt_mem, struct df *test, int predicts[]);
float accuracy(struct df *test, int predicts[]);




// ------------------
// STATIC ALLOCATION
// ------------------
struct df _df;
struct dt_mem _dt;
alignas(CACHE_LINE) int _predicts[MAX_CAPACITY];



// ---------------
// DEFINITIONS
// ---------------
int main(int argc, char **argv){
    clock_t t;

    read_df(argv[2], &_df);
    read_dtree(argv[1], &_dt);
    
    t = clock();
    predict(&_dt, &_df, _predicts);
    t = clock() - t;
    
    printf("Acc: %f\n", accuracy(&_df, _predicts));
    printf("Clocks: %ld\n", t);
    
    return 0;
}

void read_dtree(const char *path, struct dt_mem *mem){
    FILE *file;
    file = fopen(path, "rb");    
    if(file == NULL){ printf("Can't open file!"); exit(1); }
    fread(mem, sizeof(struct dt_mem), 1, file);
    fclose(file);
}


void predict(struct dt_mem *dt_mem, struct df *test, int predicts[]){
    int i;

    #pragma omp parallel for
    for(i = 0; i<test->size; ++i){
    	int curr;
    	struct dt *currn;
    	float (*pixel)[PIXELS];

    	curr = 1;
    	pixel = test->pixels[i];
    	
    	while(1){
    		currn = &(dt_mem->mem[curr]);
    		if(currn->label != -2){
    			predicts[i] =  currn->label;
    			break;
    		}else if(currn->part < (*pixel)[currn->dim]){
    			curr = DT_LEFT(curr);	
    		}else{
    			curr = DT_RIGHT(curr);
    		}
    	}
    }
}


float accuracy(struct df *test, int predicts[]){
    int i, test_sz, equal, sample;
    float acc;

    acc = 0;
    test_sz = test->size;
    sample = 32;
    for(i = 0; i<test_sz; ++i){
    	equal = (test->label[i]) - predicts[i];
    	if(equal) equal = 0; else equal = 1;
        acc += equal;
        if(sample-- > 0) printf("%d == %d ? %d\n", (test->label[i]), predicts[i], equal);
    }

    return acc/test_sz;
}
