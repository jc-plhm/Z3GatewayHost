#!/bin/bash
DEVICE=$DEVICE_TTY

echo "Building Z3GatewayHost start ..."
cd src/Z3GatewayHost
rm -rf build/exe
make
cd build/exe

arch=$(uname -m)

echo "ARCH: "
echo $arch
echo "DEVICE: "
echo $DEVICE

./Z3GatewayHost_Sectronic -n 0 -p $DEVICE_TTY




