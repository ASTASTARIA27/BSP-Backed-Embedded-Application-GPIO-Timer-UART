A modular, power-conscious firmware application built for the STM32F407 (Discovery Board). This project demonstrates a clean separation between hardware-specific drivers (BSP) and high-level application logic.
PROJECT GOALS -
  The application performs three primary tasks concurrently without using blocking delays:
    LED Heartbeat: Blinks the onboard LED (PD12) every 1 second
    Status Reporting: Sends a system status message via USART2 (PD5/PD6) every 5 seconds.
    Non-Blocking Execution: Uses a SysTick-based timer flag system to ensure the CPU is never stuck in a "busy-wait" loop.
Board Support Package (BSP) Layer-
  Hardware Abstraction: All register-level operations (RCC, GPIO, UART, SysTick) are encapsulated within bsp.c
  Zero Register Access in App: The application logic never touches raw memory addresses (e.g., 0x40020C00), preventing tight coupling with the hardware
  Peripheral Config: Handles the 42 MHz APB1 clock math for a precise 115200 baud rate.
Application Layer-
  Super-Loop Logic: Uses BSP_getTick() to poll for time-based events.
  Maintainability: High-level logic remains readable and can be easily ported to a different MCU by only swapping the BSP implementation
