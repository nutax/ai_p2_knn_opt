gcc -Iinclude src/split_df.c -o bin/split_df.o  
cd bin
./csv2df.o ../data/train.o ../data/test.o ../data/df.o 75 99
cd ..
