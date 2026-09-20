# TAPAS Kernel

The TAPAS kernel provides the operating-system and hardware-facing services used by the flight software. It targets ARM Cortex-M processors and integrates FreeRTOS, FATFS, CMSIS, and board-specific HAL implementations.

## Architecture

The stable API consumed by applications and middleware is exported from `include/`. Internal code is organised into components:

| Component | Responsibility |
|-----------|----------------|
| `core` | Tasks, system objects, interrupts, signals, system calls, and OS integration. |
| `drivers` | Memory and peripheral abstractions plus vendor-specific wrappers. |
| `fdir` | Fault detection, context capture, and stack tracing. |
| `file-system` | FATFS integration and file-system services. |
| `monitoring` | Console, indicators, build information, and system monitoring. |
| `platform` | Cache, MPU, and watchdog support. |
| `time` | Kernel time services and conversions. |

Within `components/<name>/`, headers at the component root form the API shared with other kernel components. `inc/` contains private headers and `src/` contains implementation files. Driver implementations selected for a chip vendor live under `components/drivers/src/*/wrapper-<vendor>/`.

Board support packages are stored under `bsp/<board>-BSP/`. Third-party code is kept under `third-parties/` and is not part of the TAPAS component layout.

## Building as part of the flight software

The normal workflow is driven from the flight-software repository root:

```bash
make default_defconfig
make kernel
```

The top-level `make menuconfig` sources `gen/Kconfig.options` into the integrated firmware configuration. The parent build passes that configuration to the kernel; `autoconf.h` contains only symbols owned by the kernel and is not rewritten when those symbols are unchanged. Kernel objects, generated configuration, libraries, and build state are written below the top-level `build/` directory.

## Standalone build

The kernel can also be configured and built independently:

```bash
make default_defconfig
make -j"$(nproc)"
```

Useful standalone targets are:

| Command | Description |
|---------|-------------|
| `make`, `make all` | Generate configuration and build HAL, FATFS, FreeRTOS, and the kernel archive. |
| `make clean` | Remove generated configuration and all kernel build domains. |
| `make <name>_defconfig` | Load a kernel defconfig. |
| `make menuconfig` | Edit the kernel configuration. |
| `make hal`, `make fatfs`, `make freertos`, `make kernel` | Build one domain. |
| `make <domain>-clean` | Clean one domain. |
| `make print-<variable>` | Print an effective Make variable. |
| `make verif` | Run static analysis. |
| `make format` | Format kernel-owned C and header files. |

The supported toolchain is provided by the parent project's Docker image. See the main flight-software README for setup instructions.

## Configuration and generated files

`gen/Kconfig` is the standalone entry point, `gen/Kconfig.platform` describes standalone target selection, and `gen/Kconfig.options` contains the kernel options shared with the parent menu. Defconfigs remain under `configs/`. Kernel and BSP generators write their outputs to `build/kernel/conf/`; these files must not be edited manually. Effective build settings are recorded in `build/state/kernel.flags`, `hal.flags`, `fatfs.flags`, and `freertos.flags`.

## Quality and contribution

Kernel code follows the repository MISRA-oriented coding rules, is compiled with warnings as errors, and is checked with Cppcheck and clang-format. See [`CODING_GUIDELINES.md`](CODING_GUIDELINES.md) before contributing.

Copyright and redistribution terms are described in [`COPYRIGHT.md`](COPYRIGHT.md).
