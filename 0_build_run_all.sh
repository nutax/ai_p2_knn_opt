mkdir -p bin
mkdir -p data
./1_csv2df.sh
./2_split_df.sh
./3_build_kdtree.sh
./4_predict_1nn.sh
