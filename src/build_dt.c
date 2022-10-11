// ---------------
// INCLUDES
// ---------------
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "df.h"
#include "dt.h"



// ---------------
// CONSTANTS
// ---------------




// ---------------
// DECLARATIONS
// ---------------
int main(int argc, char **argv);
void write_dtree(const char *path, struct dt_mem *mem);



// ------------------
// STATIC ALLOCATION
// ------------------
struct df _df;
struct dt_mem _dt;


// ---------------
// DEFINITIONS
// ---------------
int main(int argc, char **argv){
    read_df(argv[2], &_df);
    dt_build(&_dt, &_df);
    write_dtree(argv[1], &_dt);
    return 0;
}

void write_dtree(const char *path, struct dt_mem *mem){
    FILE *file;
    file = fopen(path, "wb");    
    if(file == NULL){ printf("Can't open file!"); exit(1); }
    fwrite(mem, sizeof(struct dt_mem), 1, file);
    fclose(file);
}
