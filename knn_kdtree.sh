gcc -Iinclude src/knn_kdtree.c -o bin/knn_kdtree.o -fopenmp -O3
cd bin
./knn_kdtree.o ../data/df.o 75 99
#gdb ./knn_kdtree.o
#run ../data/df.o 75 99
cd ..
