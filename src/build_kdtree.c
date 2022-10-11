// ---------------
// INCLUDES
// ---------------
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "df.h"
#include "kdt.h"



// ---------------
// CONSTANTS
// ---------------




// ---------------
// DECLARATIONS
// ---------------
int main(int argc, char **argv);
void write_kdtree(const char *path, struct kdt_mem *mem);



// ------------------
// STATIC ALLOCATION
// ------------------
struct df _df;
struct kdt_mem _kdt;


// ---------------
// DEFINITIONS
// ---------------
int main(int argc, char **argv){
    read_df(argv[2], &_df);
    kdt_build(&_kdt, &_df);
    write_kdtree(argv[1], &_train);
    return 0;
}

void write_kdtree(const char *path, struct kdt_mem *mem){
    FILE *file;
    file = fopen(path, "wb");    
    if(file == NULL){ printf("Can't open file!"); exit(1); }
    fwrite(mem, sizeof(struct kdt_mem), 1, file);
    fclose(file);
}
