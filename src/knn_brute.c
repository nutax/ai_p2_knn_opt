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
alignas(CACHE_LINE) int _predicts[MAX_CAPACITY];




// ---------------
// DEFINITIONS
// ---------------
int main(int argc, char **argv){
    clock_t t;

    srand(atoi(argv[3]));

    read_df(argv[1], &_df);
    
    split_train_test(&_df, &_train, &_test, atoi(argv[2]));
    
    t = clock();
    query_knn(&_train, &_test, _predicts);
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

void query_knn(struct df *train, struct df *test, int predicts[]){
    
    
    #pragma omp parallel shared(train, test, predicts) num_threads(80)
    {
    
    int i, j, k, train_sz, test_sz, nn;
    float diff, d, nd, (*train_pixels)[PIXELS], (*test_pixels)[PIXELS];
    

    train_pixels = train->pixels;
    test_pixels = test->pixels;

    train_sz = train->size;
    test_sz = test->size;

    #pragma omp for
    for(i = 0; i<test_sz; ++i){
        nn = 0;
        nd = 10000;
        for(j = 0; j<train_sz; ++j){
            d = squared_dist(test_pixels[i], train_pixels[j]);
            if(d < nd){
                nd = d;
                nn = j;
            }
        }
        predicts[i] = train->label[nn];
    }
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
    int i, test_sz, sample, equal;
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
