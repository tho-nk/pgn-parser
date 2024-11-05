export binary=pgn_parser
cmake -B build -DCMAKE_BUILD_TYPE=Debug
make -C build -j${nproc}
cp ./build/${binary} ./pytest/
