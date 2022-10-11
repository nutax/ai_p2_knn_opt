// ---------------
// INCLUDES
// ---------------
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdalign.h>
#include <time.h>
#include "df.h" // Data info
#include "kdt.h"




// ---------------
// CONSTANTS
// ---------------




// ---------------
// DECLARATIONS
// ---------------
int main(int argc, char **argv);
void read_kdtree(const char *path, struct kdt_mem *mem);
void query_knn(struct kdt_mem *kdt_mem, struct df *test, int predicts[]);
float accuracy(struct df *test, int predicts[]);




// ------------------
// STATIC ALLOCATION
// ------------------
struct df _df;
struct kdt_mem _kdt;
alignas(CACHE_LINE) int _predicts[MAX_CAPACITY];



// ---------------
// DEFINITIONS
// ---------------
int main(int argc, char **argv){
    clock_t t;

    read_df(argv[2], &_df);
    read_kdtree(argv[1], &_kdt);
    
    t = clock();
    query_knn(&_kdt, &_df, _predicts);
    t = clock() - t;
    
    printf("Acc: %f\n", accuracy(&_df, _predicts));
    printf("Clocks: %ld\n", t);
    
    return 0;
}

void read_kdtree(const char *path, struct kdt_mem *mem){
    FILE *file;
    file = fopen(path, "rb");    
    if(file == NULL){ printf("Can't open file!"); exit(1); }
    fread(mem, sizeof(struct kdt_mem), 1, file);
    fclose(file);
}


void query_knn(struct kdt_mem *kdt_mem, struct df *test, int predicts[]){
    int i;

    #pragma omp parallel for
    for(i = 0; i<test->size; ++i){
    	struct kdt *nn;
    	
		nn = NULL;
    	kdt_query_knn(kdt_mem, test->pixels[i], &nn);
    	if(nn != NULL) predicts[i] = nn->label;
    	else{
    	predicts[i] = -1;
    		//printf("Failed at %d iteration\n\n\n", i);
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
