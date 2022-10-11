gcc -Iinclude src/knn_brute.c -o bin/knn_brute.o -fopenmp
cd bin
./knn_brute.o ../data/df.o 80 0
cd ..
