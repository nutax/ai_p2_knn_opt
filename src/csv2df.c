// ---------------
// INCLUDES
// ---------------
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "df.h"



// ---------------
// CONSTANTS
// ---------------
#define MAX_LINE_SZ (PIXELS*6)



// ---------------
// DECLARATIONS
// ---------------
int main(int argc, char **argv);
void read_csv(const char *path, struct df *df);
void write_df(const char *path, struct df *df);



// ------------------
// STATIC ALLOCATION
// ------------------
struct df df;
char line[MAX_LINE_SZ];



// ---------------
// DEFINITIONS
// ---------------
int main(int argc, char **argv){
    int i;

    df_init(&df);
    for(i = 2; i<argc; ++i){
        read_csv(argv[i], &df);
    }
    write_df(argv[1], &df);
    return 0;
}

void read_csv(const char *path, struct df *df){
    int label;
    float pixel;
    char *token, *aux;
    int i, j;
    FILE *stream;
    
    stream = fopen(path, "r");
    

    for (i = df->size; fgets(line, MAX_LINE_SZ, stream); ++i)
    {
        token = strtok(line, ",");
        df->label[i] = atoi(token);
        token = strtok(NULL, ",");
        
        for(j = 0; token; ++j){
            df->pixels[i][j] = (float) atoi(token);
            token = strtok(NULL, ",");
        }
    }

    df->size = i;
    fclose(stream);
}

void write_df(const char *path, struct df *df){
    FILE *file;
    file = fopen(path, "wb");    
    if(file == NULL){ printf("Can't open file!"); exit(1); }
    fwrite(df, sizeof(struct df), 1, file);
    fclose(file);
}
