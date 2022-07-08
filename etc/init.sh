#!/bin/bash

echo "Building Z3GatewayHost start ..."
cd src/Z3GatewayHost
rm -rf build
make
cd build/exe

arch=$(uname -m)

rm -rf /app/output/$arch
mkdir -p /app/output/$arch
mv Z3GatewayHost_Sectronic Z3GatewayHost
cp Z3GatewayHost /app/output/$arch
rm  -rf /app/src/Z3GatewayHost/build



