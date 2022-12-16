#!/bin/bash

echo "Building Z3GatewayHost start ..."
cd src/Z3GatewayHost
cd build/exe

arch=$(uname -m)

echo "ARCH: "
echo $arch
DEVICE=$DEVICE_TTY
echo "DEVICE: "
echo $DEVICE

./Z3GatewayHost_Sectronic -n 0 -p $DEVICE



