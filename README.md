- [Z3Gateway Host BUILD](#floppy_disk-z3gateway-host-build)
  - [build Z3GatewayHost](#build-z3gatewayhost)
  - [run Z3GatewayHost](#run-z3gatewayhost)
  - [example of comands](#example-of-comands)
- [Z3Gateway Host DOCKER BUILD](#floppy_disk-z3gateway-host-docker-build)
  - [configure docker Z3GatewayHost](#configure-docker)
  - [build docker Z3GatewayHost](#build-docker-z3gatewayhost)
  - [run docker Z3GatewayHost](#run-docker-z3gatewayhost)
  - [docker logs Z3GatewayHost](#docker-logs-z3gatewayhost)
  - [docker attach Z3GatewayHost](#docker-attach-z3gatewayhost)
- [Z3GatewayHost Commands](#z3gatewayhost-commands)
  - [info](#info)
  - [create-network](#create-network)
  - [open-network](#open-network-no-install-code)
  - [broad-pjoin](#network-broad-pjoin)
  - [close-network](#close-network)
-[Device Commands](#device-commands)
  - [device-table](#display-device-table)
  - [bind](#bind)
  - [binding table](#display-binding-table)
  - [read attribute](#read-attribute)
  - [configure reporting](#configure-a-device-reporting)
  - [remove device](#remove-device-from-zigbee-network)
- [OnOff commands](#onoff-commands)
  - [On Command](#on-command)
  - [Toggle Command](#toggle-command)
  - [Off Command](#off-command)
- [NCP](#ncp)
  - [Generate Firmware](#generate-firmware)
  - [Flash Firmware](#flash-firmware)

- [Usefull Links](#usefull-links)

# :floppy_disk: Z3Gateway Host BUILD
### Build Z3GatewayHost

> :warning:: you might need to install libreadline-dev lib32ncurses5-dev before building the project

```sh
$ apt-get install libreadline-dev
$ apt-get install lib32ncurses5-dev

```

> to build de the project just go to the project files and run make

```sh
cd src/Z3GatewayHost
make
```

### Run Z3GatewayHost

the project is generated at src/Z3GatewayHost/build/exe/Z3GatewayHost_Sectronic.exe

just run it with ```./Z3GatewayHost_Sectronic.exe -n 0 -p /dev/ttyUSB0```


### example of comands

- create network

```
plugin network-creator start 1
```

- display device-table

```
plugin device-table print
```

- open network

```
plugin network-creator-security open-network
```

- send ON command

```
zcl on-off on
plugin device-table send {<deviceEui>} <deviceEndpoint>
```

# :floppy_disk: Z3Gateway Host DOCKER

## configure docker

set your device tty in .env with DEVICE_TTY=/dev/my_efr

```sh
echo "DEVICE_TTY=/dev/my_efr" > .env
```

## build docker Z3GatewayHost 

run 
```sh 
./manage.sh build
```

## run docker Z3GatewayHost 

run 
```sh 
./manage.sh run
```

## docker logs Z3GatewayHost

run 
```sh 
docker logs -f --tail=100 z3host
```

## docker attach Z3GatewayHost

Use this command to attach to Z3GatewayHost CLI

run 
```sh 
docker attach z3host
```

# :blue_book: Z3GatewayHost Commands
#  Gateway

## info: 

CLI: `info` 

description: get Gateway information 

# Network Commands

## create network: 

CLI: `plugin network-creator start 1`

description: create zigbee network 

## open network (no install code): 

CLI: `plugin network-creator-security open-network`

description: open-network

## network broad-pjoin: 

CLI: `network broad-pjoin 0x78` 

CLI params: `network broad-pjoin <seconds:2>`

description: open network

## close network: 

CLI: `plugin network-creator-security close-network`

description: close network


# Device Commands
## display device-table: 

CLI: `plugin device-table print`

description: display devices paired to the gateway

## bind: 

CLI: `zdo bind 0xF563 0x2 0x1 0x0006 {84B4DBFFFE27C26E} {588E81FFFE9A3394}` 

CLI params: `zdo bind <nodeID:2> <deviceEP:1> <gatewayEP:1> <clusterID:2> <deviceEUI64> <gatewayEUI64>`

description: bind and endpoint cluster to the gateway

## display binding table: 

CLI: `zdo mgmt-bind 0xFB9E 0`

CLI params: `zdo mgmt-bind <nodeID:2> <statartIndex>`

description: display binding table for a given device

## read attribute

CLI:
```
zcl global read 0x0000 0x0000
plugin device-table send {086BD7FFFE5CB4C4} 1
```

CLI params:
```
zcl global read <cluserID:2> <attributeID:2>
plugin device-table send {<deviceEUI64>} <deviceEP:1>
```

description: read cluster attibute value of a device

## configure a device reporting 

CLI:
```
zcl global send-me-a-report 0x0006 0x0000 0x10 0x0001 0x0078 {01}
plugin device-table send {086BD7FFFE5CB4C4} 1
```

CLI params: 
```
zcl global send-me-a-report <clusterID:2> <attributeId:2> <dataType:1> <minReportTime:2> <maxReportTime:2> <reportableChange:->1
plugin device-table send {<deviceEUI64>} <deviceEP:1>
```

# OnOff Commands

## On Command

CLI: `zcl on-off on`
CLI: `plugin device-table send {<deviceEui>} <deviceEndpoint>`

## Toggle Command

CLI: `zcl on-off toggle`
CLI: `plugin device-table send {<deviceEui>} <deviceEndpoint>`

## Off Command

CLI: `zcl on-off off`
CLI: `plugin device-table send {<deviceEui>} <deviceEndpoint>`

# NCP

You can find already generated ncp application and bootload images in Z3GatwayHost/bootloader

### Generate Firmware
For the board EF32xG21 2.4 GHz 10dBm RB (efr32mg21a020f1024im32-brd4180a) we need to create a new project uart xmodem.

- Plug the board to your computer
- In SimplicityStudio, [File] > [New] > [SiliconLabs Project Wizard]
- ensure your board is connected
- [Next]
- filter on zigbee examples project
- select the NCP UART HW (Hardware Flow Control)
- [Next]
- [Finish]
- Generate project files
- Build project

### Flash Firmware

application image path: `C:\...\SimplicityStudio\v5_workspace\ncp-uart-hw\GNU ARM v10.2.1 - Default\ncp-uart-hw.s37`

bootloader image path: `C:\SiliconLabs\SimplicityStudio\v5\developer\sdks\gecko_sdk_suite\v3.2\platform\bootloader\sample-apps\bootloader-storage-internal-single\efr32mg21a020f1024im32-brd4180a\bootloader-storage-internal-single.s37`

- Plug the board to your computer
- right click on the board, select [Upload Application]
- select your application image
- select the bootloader image
- select erase chip
- [OK]
- unplug and re plug your board

# usefull links

- [Silabs light and switch](https://community.silabs.com/s/article/zigbee-3-0-tutorial-light-and-switch-from-scratch-step-4?language=en_US)
- [Tutorial for Install Code](https://github.com/MarkDing/IoT-Developer-Boot-Camp/wiki/Zigbee-Hands-on-Forming-and-Joining#71-programming-the-install-code-to-switch-router-device)
- [AN1089: zigbee intall code](https://www.silabs.com/documents/public/application-notes/an1089-using-installation-codes-with-zigbee-devices.pdf)
- [Silabs Commander tool](https://community.silabs.com/s/article/simplicity-commander?language=en_US)
- [zigbee-af-api](https://docs.silabs.com/d/zigbee-af-api/6.9/zdo#zig-bee-device-object-commands-zdo)