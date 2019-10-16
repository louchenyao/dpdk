#! /bin/bash

sudo apt install -y libnuma-dev


make config -j8 T=x86_64-native-linux-gcc
make install -j8 T=x86_64-native-linux-gcc