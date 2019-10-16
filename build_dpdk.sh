#! /bin/bash

sudo apt install -y libnuma-dev

make install -j8 T=x86_64-native-linux-gcc