gcc -Iinclude src/csv2df.c -o bin/csv2df.o  
cd bin
./csv2df.o ../data/df.o ../data/train.csv ../data/test.csv
cd ..
