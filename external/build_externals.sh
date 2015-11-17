#! /bin/bash

pushd cuba &> /dev/null
./configure --prefix=$PWD/../
make install -j4
popd &> /dev/null
