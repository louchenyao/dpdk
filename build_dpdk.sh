#! /bin/bash

sudo apt install -y libnuma-dev

cd ..
make install -j8 T=x86_64-native-linux-gcc