#!/bin/bash

echo "Building Z3GatewayHost start ..."
cd src/Z3GatewayHost
rm -rf build
make
cd build/exe

arch=$(uname -m)

echo "ARCH: "
echo $arch
DEVICE=$DEVICE_TTY
rcho "DEVICE: "
echo $DEVICE

./Z3GatewayHost -n 0 -p -n 0 -p $DEVICE



