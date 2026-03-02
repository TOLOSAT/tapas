# TAPAS Kernel — TOLOSAT Autonomous Payload & Avionics Software

## 1. Overview

The **TAPAS Kernel** is the core component of the TOLOSAT flight software. It provides all low-level services required for the autonomous and reliable operation of the satellite. Designed as a **monolithic, real-time operating system (RTOS)**, the kernel ensures deterministic behavior, fault tolerance, and a high level of integration between critical software components.

The kernel acts as the bridge between the hardware and user-level applications. It handles task scheduling, memory management, peripheral interfaces, telemetry, fault recovery, and system supervision.

## 2. Objectives

The main purposes of the TAPAS kernel are:
- Provide a **real-time execution environment** using FreeRTOS.
- Manage **hardware resources** such as memory, I/O devices, and timers.
- Implement **Failure Detection, Isolation and Recovery (FDIR)** mechanisms.
- Ensure **deterministic and safe execution** through static resource allocation.
- Offer a consistent **Device API** for communication between applications and kernel-managed objects.
- Provide **monitoring and diagnostic capabilities** including stack traces and system snapshots.

## 3. Architecture

The TAPAS kernel follows a **monolithic architecture** built on top of mature third-party components. It integrates multiple functionalities within a single software space for better control, reduced complexity, and deterministic operation.

### 3.1 Third-Party Dependencies
- **FreeRTOS** — multitasking, scheduling, inter-task synchronization.
- **FATFS** — file system management for non-volatile memory.
- **HALs (Hardware Abstraction Layers)** — vendor-provided interfaces for hardware peripherals.

### 3.2 Internal Layers
| Layer      | Description                                                                                |
|------------|--------------------------------------------------------------------------------------------|
| **Core**   | Low-level kernel infrastructure: task and memory management, system objects, system calls. |
| **DRV**    | Hardware drivers and peripheral abstractions.                                              |
| **FDIR**   | Failure detection, isolation and recovery logic.                                           |
| **System** | Diagnostics, monitoring, logging, and housekeeping services.                               |

### 3.3 Key Features
- Multitasking with task, mutex, and signal management.
- Static resource allocation to ensure deterministic execution.
- File system access through FATFS.
- Fault management and system context capture.
- Console interface and logging system.
- Real-time clock and timer management (CUC-compliant).
- On-the-fly flight software image switching.
- Watchdog integration.
- Planned extensions: event system, housekeeping service, and enhanced task isolation.

## 4. Development Environment

The kernel uses the same development and build infrastructure as the main TAPAS project.

### 4.1 Requirements
A Linux-based environment is required (Ubuntu 22.04 recommended). Docker usage is advised for consistency.

**Dependencies:**
- build-essential
- cppcheck (v2.7)
- kconfig-frontends
- doxygen
- gcc-arm-none-eabi (v10.3.1)
- gdb-multiarch
- git
- graphviz
- openocd

### 4.2 Docker Usage
```bash
git clone https://github.com/TOLOSAT/flight-software.git
cd flight-software/kernel
./run-docker.sh
```

This command builds and runs the TAPAS kernel Docker container. You can attach Visual Studio Code or connect via terminal.

## 5. Build System and Usage

The kernel build is handled through the project-wide Makefile system.

| Command                          | Description                                                 |
|----------------------------------|-------------------------------------------------------------|
| `make`, `make all`, `make build` | Build the kernel.                                           |
| `make menuconfig`                | Configure kernel options (target board, debug level, etc.). |
| `make pre-build`                 | Generate precompiled sources based on configuration files.  |
| `make clean`                     | Remove all build artifacts.                                 |
| `./update-doc.sh`                | Generate or update Doxygen documentation.                   |

## 6. Coding Standards and Quality Assurance

The kernel is written entirely in **C** and developed according to best practices for safety-critical embedded systems:

- Compliance with **MISRA-C:2012** rules.
- Partial compliance with **ECSS-E-ST-40C** and **ECSS-Q-ST-80C** standards.
- Static analysis using **cppcheck**.
- Automatic documentation with **Doxygen**.
- Code formatting enforced by **clang-format**.
- Version control and peer review via **Git**.

## 7. Documentation

- Technical note: *TOLOSAT_TN_13_25 – TAPAS Flight Software Architecture and Kernel Description*
- Reference Standards:
  - **ECSS Standards**:
    - ECSS-E-ST-40C — Software Engineering
    - ECSS-Q-ST-80C — Software Product Assurance
    - ECSS-E-ST-70-41C — Packet Utilization Standard (PUS)
  - **SAVOIR Standards**

## 8. Acronyms

| Acronym | Definition                                         |
|---------|----------------------------------------------------|
| API     | Application Programming Interface                  |
| BSP     | Board Support Package                              |
| CCSDS   | Consultative Committee for Space Data Systems      |
| CMSIS   | Cortex Microcontroller Software Interface Standard |
| CUC     | CCSDS Unsegmented Time Code                        |
| ECSS    | European Cooperation for Space Standardization     |
| FDIR    | Failure Detection, Isolation and Recovery          |
| HAL     | Hardware Abstraction Layer                         |
| MC      | Monitoring & Control                               |
| NVM     | Non-Volatile Memory                                |
| OS      | Operating System                                   |
| PUS     | Packet Utilization Standard                        |
| PS      | Packet Store                                       |
| RTOS    | Real-Time Operating System                         |
| TAPAS   | TOLOSAT Autonomous Payload and Avionic Software    |
| TC      | TeleCommand                                        |
| TM      | TeleMetry                                          |

## 9. Authors and Maintainers

- **Merlin Kooshmanian** — Flight Software Architect
- **Theo Bessel** — Flight Software Maintainer

For technical questions, please contact the TOLOSAT Flight Software team.
