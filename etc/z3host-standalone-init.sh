#!/bin/bash

ARCH=$TARGET_ARCHITECTURE
DEVICE=$DEVICE_TTY
echo $DEVICE
echo $ARCH
./bin/$ARCH/Z3GatewayHost -n 0 -p $DEVICE