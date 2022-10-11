gcc -Iinclude src/predict_1nn.c -o bin/predict_1nn.o -fopenmp -O3
cd bin
./predict_1nn.o ../data/kdtree.o ../data/test.o
#gdb ./knn_kdtree.o
#run ../data/df.o 75 99
cd ..
