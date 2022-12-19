#!/bin/bash

ARCH=$TARGET_ARCHITECTURE
DEVICE=$DEVICE_TTY
echo $DEVICE
echo $ARCH
./bin/aarch64/Z3GatewayHost -n 0 -p $DEVICE