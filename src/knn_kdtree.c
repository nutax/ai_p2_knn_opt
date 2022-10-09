// ---------------
// INCLUDES
// ---------------
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdalign.h>
#include <time.h>
#include <omp.h>
#include "df.h" // Data info
#include "kdt.h"




// ---------------
// CONSTANTS
// ---------------




// ---------------
// DECLARATIONS
// ---------------
int main(int argc, char **argv);
void split_train_test(struct df *df, struct df *train, struct df *test, int train_p);
void query_knn(struct df *train, struct df *test, int predicts[]);
float squared_dist(float a[], float b[]);
float accuracy(struct df *test, int predicts[]);




// ------------------
// STATIC ALLOCATION
// ------------------
struct df _df;
struct df _train;
struct df _test;
struct kdt_mem _kdt;
alignas(CACHE_LINE) int _predicts[MAX_CAPACITY];




// ---------------
// DEFINITIONS
// ---------------
int main(int argc, char **argv){
    clock_t t;

    srand(atoi(argv[3]));
    
    read_df(argv[1], &_df);
    
    split_train_test(&_df, &_train, &_test, atoi(argv[2]));

    kdt_build(&_kdt, &_test);
    
    t = clock();
    query_knn(&_kdt, &_test, _predicts);
    t = clock() - t;
    
    printf("Acc: %f\n", accuracy(&_test, _predicts));
    printf("Clocks: %ld\n", t);
    
    return 0;
}


void split_train_test(struct df *df, struct df *train, struct df *test, int train_p){
    int p, i, j, df_sz, train_sz, test_sz;

    p = train_p*(RAND_MAX/100);
    df_sz = df->size;
    train_sz = 0;
    test_sz = 0;

    for(i = 0; i<df_sz; ++i){
        if(rand() < p){
            train->label[train_sz] = df->label[i];
            for(j = 0; j<PIXELS; ++j){
                train->pixels[train_sz][j] = df->pixels[i][j];
            }
            ++train_sz;
        }else{
            test->label[test_sz] = df->label[i];
            for(j = 0; j<PIXELS; ++j){
                test->pixels[test_sz][j] = df->pixels[i][j];
            }
            ++test_sz;
        }
    }

    train->size = train_sz;
    test->size = test_sz;

    printf("Train rows: %d\nTest rows: %d\n", train_sz, test_sz);
}

void query_knn(struct kdt_mem *kdt_mem, struct df *test, int predicts[]){
    int i;
    struct kdt *nn;

    for(i = 0; i<test->size; ++i){
    	kdt_query_knn(kdt_mem, test->pixels[i], &nn);
    	predicts[i] = nn->label;
    }
}

// [1 2 3 4 5] -> train: [1 1 2] test: [3 4 5]


float squared_dist(float a[], float b[]){
    int i;
    float aux[PIXELS], sum;

    #pragma omp simd aligned(a,b: 64)
    for(i = 0; i<PIXELS; ++i){
        aux[i] = a[i] - b[i];
        aux[i] = aux[i]*aux[i];
    }

    sum = 0;
    for(i = 0; i<PIXELS; ++i) {
        sum += aux[i];
    }
    return sum;
}


float accuracy(struct df *test, int predicts[]){
    int i, test_sz;
    float acc;

    acc = 0;
    test_sz = test->size;
    for(i = 0; i<test_sz; ++i){
        acc += ((test->label[i]) == predicts[i]) ? 1 : 0;
    }

    return acc/test_sz;
}
