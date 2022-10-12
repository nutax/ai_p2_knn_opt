gcc -Iinclude src/predict_dt.c -o bin/predict_dt.o -fopenmp -O3
cd bin
./predict_dt.o ../data/dtree.o ../data/test.o
#gdb ./knn_kdtree.o
#run ../data/df.o 75 99
cd ..
