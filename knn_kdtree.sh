gcc -g -Iinclude src/knn_kdtree.c -o bin/knn_kdtree.o -fopenmp
cd bin
#gdb ./knn_kdtree.o ../data/df.o 75 99
gdb ./knn_kdtree.o
run ../data/df.o 75 99
cd ..
