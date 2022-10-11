// ---------------
// INCLUDES
// ---------------
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "df.h"




// ---------------
// CONSTANTS
// ---------------




// ---------------
// DECLARATIONS
// ---------------
int main(int argc, char **argv);
void write_df(const char *path, struct df *df);
void split_train_test(struct df *df, struct df *train, struct df *test, int train_p);




// ------------------
// STATIC ALLOCATION
// ------------------
struct df _df;
struct df _train;
struct df _test;


// ---------------
// DEFINITIONS
// ---------------
int main(int argc, char **argv){
	srand(atoi(argv[5]));
    read_df(argv[3], &_df);
    split_train_test(&_df, &_train, &_test, atoi(argv[4]));
    write_df(argv[1], &_train);
    write_df(argv[2], &_test);
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


void write_df(const char *path, struct df *df){
    FILE *file;
    file = fopen(path, "wb");    
    if(file == NULL){ printf("Can't open file!"); exit(1); }
    fwrite(df, sizeof(struct df), 1, file);
    fclose(file);
}
