rm -rf build
mkdir build
cd build
cmake -j8 -DCMAKE_CXX_COMPILER=/usr/bin/g++-9 ..
make -j8