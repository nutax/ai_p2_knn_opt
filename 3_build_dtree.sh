gcc -Iinclude src/build_dt.c -o bin/build_dt.o -fopenmp -O3
cd bin
./build_dt.o ../data/dtree.o ../data/train.o
#gdb ./knn_kdtree.o
#run ../data/df.o 75 99
cd ..
