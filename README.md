
## Project: STM32F407 Bare-Metal BSP & Virtualized UART Monitor

This repository contains a bare-metal C implementation for the **STM32F407 Discovery Board** (ARM Cortex-M4). The project demonstrates low-level hardware abstraction by interfacing with the Nested Vectored Interrupt Controller (NVIC) for SysTick timing and implementing a custom Universal Synchronous/Asynchronous Receiver Transmitter (USART) driver from scratch. The entire firmware lifecycle—from register-level coding to compilation and execution—is validated using the **Renode** simulation framework.

## Technology Stack
* **Target Hardware:** STM32F407VGT6 (ARM Cortex-M4)
* **Clock Speed:** 16 MHz (Internal HSI)
* **Language:** C (Bare-Metal)
* **Toolchain:** ARM GNU Toolchain (`arm-none-eabi-gcc`)
* **Simulation:** Renode (Antmicro)
* **Key Peripherals:** * **SysTick Timer:** Used for precise 1ms heart-beat timing.
    * **USART2:** Configured for 115200 baud serial communication.
    * **GPIO Port D:** Used for onboard LED status indication (Pin 12).
    * **GPIO Port A:** Used for USART2 Alternate Function mapping (Pins 2 & 3).

## System Logic & Implementation

The application utilizes a **Board Support Package (BSP)** architecture to separate hardware-specific register logic from the high-level application logic.

### 1. Precision Timing (SysTick)
To achieve a stable **1ms time-base** for the system, the Cortex-M4 SysTick timer is configured using the system clock frequency:
* **Reload Value:** Calculated as $f_{sys} / 1000 - 1$.
* **Interrupt Handler:** The `SysTick_Handler` increments a global `ms_ticks` variable, providing a non-blocking timing reference for the super-loop.

### 2. Serial Communication (USART2)
Instead of using high-level HAL libraries, the UART driver interacts directly with the **APB1** bus:
* **Baud Rate Generation:** The Baud Rate Register (BRR) is manually calculated for a 16 MHz clock to achieve **115200 baud**.
* **Alternate Function Mapping:** GPIOA Pins 2 and 3 are set to `AF7` mode to route the CPU internal signals to the virtual serial port.
* **Synchronization:** Implements a polling-based `Write` function that monitors the **TXE** (Transmit Data Register Empty) flag before pushing data to the bus.

---

## Bare-Metal Compilation & Linker Script
Compilation is performed in a **Linux environment** using the ARM Cross-Compiler. A custom linker script (`.ld`) ensures the binary is mapped correctly to the STM32 memory layout.

* **Flash Memory (`0x08000000`):** Mapped for the Vector Table, executable code (`.text`), and constants (`.rodata`).
* **SRAM (`0x20000000`):** Mapped for initialized data and stack allocation.
* **Startup Code:** A vectorized assembly file (`.s`) defines the reset handler and links the C entry point (`main`).

---

## Virtual Hardware Simulation (Renode)
The firmware is tested in a hardware-less environment using **Renode**, which simulates the entire STM32F4 SoC.

* **Platform Definition:** Uses the built-in STM32F407 platform description to attach virtual peripherals to the system bus at datasheet-correct addresses.
* **Peripheral Analysis:** A virtual **UART Analyzer** window intercepts memory writes to `USART2->DR` and renders them as text, providing real-time feedback of the system status.
* **Execution Script (`.resc`):** Automates the machine creation, resets the virtual silicon, loads the `.elf` binary, and opens the serial monitor.

---

## Key Learnings

### Hardware Abstraction via Structs
Utilized Memory-Mapped structures to define peripheral register maps. This provides a type-safe way to access hardware, replacing dangerous raw pointer arithmetic with readable, maintainable C code.

### The `volatile` Keyword
Encountered and solved issues where the compiler would optimize away hardware status loops. By marking register structures as `volatile`, the CPU is forced to perform a fresh bus read for every iteration, ensuring proper synchronization with peripheral flags.

### Linux Toolchain Mastery
Transitioned from a Windows GUI environment to a Linux-based CLI workflow. This involved mastering `arm-none-eabi-gcc` flags, understanding case-sensitive file structures, and handling cross-compilation linking errors.

---

## Code Structure
* `Src/main.c`: High-level application logic and timing loops.
* `Src/bsp.c`: Low-level register definitions and peripheral initialization.
* `Inc/bsp.h`: Structure definitions for GPIO, RCC, USART, and SysTick.
* `Startup/`: Assembly startup files for the ARM Cortex-M4 vector table.
* `stm32f4.resc`: Renode automation script for the simulation environment.
* `*.ld`: Linker scripts for FLASH and RAM memory mapping.

---

## How to Run the Simulation

**1. Build the Project:**
```bash
arm-none-eabi-gcc -c -mcpu=cortex-m4 -mthumb -I./Inc Src/main.c -o main.o
arm-none-eabi-gcc -c -mcpu=cortex-m4 -mthumb -I./Inc Src/bsp.c -o bsp.o
arm-none-eabi-gcc -c -mcpu=cortex-m4 -mthumb Startup/startup_stm32f407vgtx.s -o startup.o
arm-none-eabi-gcc main.o bsp.o startup.o -T STM32F407VGTX_FLASH.ld -mcpu=cortex-m4 -mthumb -specs=nosys.specs -o bsp.elf
```

**2. Launch Renode:**
```bash
renode stm32f4.resc
```
*The USART2 analyzer window will open automatically and begin displaying "System Status: Running", confirming successful bare-metal execution!*

***

Would you like to add a section on how to use GDB with Renode for the "How to Run" section?
