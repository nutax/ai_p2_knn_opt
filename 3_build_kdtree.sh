gcc -Iinclude src/build_kdtree.c -o bin/build_kdtree.o -fopenmp -O3
cd bin
./build_kdtree.o ../data/kdtree.o ../data/train.o
#gdb ./knn_kdtree.o
#run ../data/df.o 75 99
cd ..
