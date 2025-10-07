# TOLOSAT Autonomous Payload & Avionics Software Kernel

## Context

TAPAS (TOLOSAT Autonomous Payload and Avionic Software) is the flight software for the TOLOSAT 3U nanosatellite. This software has different roles including :
- Ensuring the vital functions of the satellite: temperature management, attitude management, memory, power and computing resources management.
- Ensuring satellite - ground communication.
- Ensuring the piloting of the payloads.

If you want to know more about the inner workings of TAPAS, you can read the [technical specifications](doc/technical-specifications/Technical_Specifications.md).

## Purpose

The TAPAS Kernel manages all the core functions of the TAPAS flight software. The kernel becomes an interface between internal mechanisms such as the file system, peripherals and multitasking and the user. It also integrates multiple formerly separate modules into a unified kernel and enforces a clear separation between user-level applications and kernel-level functionality.

## Requirements

To develop TAPAS, it is necessary to have a LINUX based OS (e.g. Ubuntu, Arch, ...) installed on your computer. The docker allows to avoid compatibility problems between Linux distribution and/or versions.

Windows and MacOS are not recommended for TAPAS development.
For MacOS, it's possible to reproduce the same environment as under Linux, as this is a UNIX OS, and therefore has a high degree of compatibility. However, you need to download all the necessary packages either manually or using HomeBrew (if available), taking care to download the correct versions of some of them.
For Windows, it's possible to reproduce the build environment with WSL, but OpenOCD doesn't work, preventing the use of a physical board. QEMU, on the other hand, works without a problem.

If you have have Docker. Just clone this repository and run the command `./run-docker.sh`. The docker image should be created and then a detached docker should be created. One can either attach VSCode into the container and develop with it, or simply attach the docker to the terminal by doing docker attach {id}. If you are not on Linux you may have trouble with `make upload` that's why Linux is recommended.

If you're on Ubuntu 22.04 and don't want to use Docker, you can install the dependencies for TAPAS installing :
- build-essential
- cppcheck (v2.7 is required)
- kconfig-frontends
- doxygen
- gcc-arm-none-eabi (v10.3.1 is required)
- gdb-multiarch (aliased into arm-none-eabi-gdb)
- git
- graphviz
- nano
- openocd
- telnet
- vim

It is then recommended to download VSCode and the TAPAS extension pack, which can be found at the following address: https://github.com/TOLOSAT/flight-software-extension-pack/tree/main/outputs. Simply download the latest .vsix file and install it with VSCode.

**NOTE :**  Compiling and running outside the docker is possible but deprecated.

## Quick Usage

To quickly use the kernel, you need to know the following commands:
- `make`, `make all` or `make build` builds the kernel, and uploads it to the board.
- `make menuconfig` helps you to select the right configuration for your use case
- `make clean` removes all previously generated files.
- `make pre-build` pre-build some sources files based on the json configuration files.
- `./run-docker.sh` builds and runs the Docker container in the background. You can use the `-a` flag in order to attach automatically.
- `./update-doc.sh` update/creates html documentation with doxygen in the build folder.

## Acronyms

<center>

| Acronym | Definition                                         |
|---------|----------------------------------------------------|
| API     | Application Program Interface                      |
| BSP     | Board Support Package                              |
| CCSDS   | Consultative Commitee for Space Data Systems       |
| CMSIS   | Cortex Microcontroller Software Interface Standard |
| CUC     | CCSDS Usegmented time Code (cf. CCSDS 301.0-B-2)   |
| ECSS    | European Cooperation for Space Standardization     |
| FDIR    | Failure Detection Identification and Recovery      |
| HAL     | Hardware Abstraction Layer                         |
| NVM     | Non Volatile Memory                                |
| OS      | Operating System                                   |
| PUS     | Packet Utilization Standard (cf. ECSS-E-ST-70-41C) |
| PS      | Packet Store                                       |
| RTOS    | Real Time OS                                       |
| TAPAS   | TOLOSAT Autonomous Payload and Avionic Software    |
| TC      | TeleCommand                                        |
| TM      | TeleMetry                                          |

</center>