export binary=mlp-ha
cmake -B build -DCMAKE_BUILD_TYPE=Debug
make -C build -j${nproc}
cp ./build/${binary} ./pytest/
