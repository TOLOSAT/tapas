# Board Support Packages

Each `bsp/<board>-BSP/` directory contains the board-specific integration used by the kernel:

- `Kconfig` describes board capabilities;
- `bsp.json` describes memories and peripherals consumed by code generators;
- `include/` contains BSP headers;
- `src/` contains startup, system, and BSP implementation files;
- `image.ld` is the linker script;
- vendor project files such as `.ioc` may be kept as hardware configuration references.

Generated peripheral and memory configuration is written to `build/kernel/conf/`, never into the BSP source tree.
