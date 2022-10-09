gcc -Iinclude src/knn_kdtree.c -o bin/knn_kdtree.o -fopenmp
cd bin
./knn_kdtree.o ../data/df.o 75 99
cd ..
