#ifndef __LAYOUT_H__
#define __LAYOUT_H__

// ---------------
// INCLUDES
// ---------------
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdalign.h>


// ---------------
// CONSTANTS
// ---------------
#define MAX_CAPACITY 35000
#define PIXELS 784
#define COLS (1+PIXELS)
#define CACHE_LINE 64
#define LABELS 26



// ---------------
// DECLARATIONS
// ---------------
struct df;
void df_init(struct df *df);
void read_df(const char *path, struct df *df);



// ---------------
// DEFINITIONS
// ---------------
struct df{
    alignas(CACHE_LINE) int size;
    alignas(CACHE_LINE) int label[MAX_CAPACITY];
    alignas(CACHE_LINE) float pixels[MAX_CAPACITY][PIXELS];
};

void df_init(struct df *df){
    df->size = 0;
}

void read_df(const char *path, struct df *df){
    FILE *file;
    file = fopen(path, "rb");    
    if(file == NULL){ printf("Can't open file!"); exit(1); }
    fread(df, sizeof(struct df), 1, file);
    fclose(file);
}

#endif
