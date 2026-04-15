#ifndef BSP_H
#define BSP_H
#define GPIO_BASE_ADDRESS 0x40020C00
#define RCC_BASE_ADDRESS  0x40023800
#define USART2_BASE_ADDRESS 0x40004400
#define SYSTICK_BASE_ADDRESS  0x0000003C
extern volatile uint32_t ms_ticks;

//gpio registers
typedef struct {
	uint32_t MODER; //offset at 0x00 (4 bytes = 32 bits)
	uint32_t OTYPER; //offset at 0x04
	uint32_t OSPEEDR; //offset at 0x08
	uint32_t PUPDR; //offset at 0x0C
	uint32_t IDR;
	uint32_t ODR;
	uint32_t BSRR;
	uint32_t LCKR;
	uint32_t AFRL;
	uint32_t AFRH;
}GPIO_t;

//same as above for the RCC
typedef struct {
	uint32_t CR; //offset at 0x00
	uint32_t PLLCFGR; //offset 0x04
	uint32_t CFGR;
	uint32_t CiR;
	uint32_t AHB1RSTR;
	uint32_t AHB2RSTR;
	uint32_t AHB3RSTR;
	uint32_t APB1RSTR;
	uint32_t APB2RSTR;
	uint32_t AHB1ENR;
	uint32_t AHB2ENR;
	uint32_t AHB3ENR;
	uint32_t APB1ENR;

}RCC_t;

//usart2 registers
typedef struct {
	uint32_t STATUS_REG; //offset at 0x00
	uint32_t DATA_REG; //offset at 0x04
	uint32_t BRR; //Baudrate register
	uint32_t CONTROL_REG1;
	uint32_t CONTROL_REG2;
	uint32_t CONTROL_REG3;
	uint32_t GTPR;
}USART2_t;

//check CortexM4 programming manual
typedef struct {
	uint32_t CONTROL; //Enables the counter and its interrupt.
	uint32_t LOAD; //The starting value (24-bit) the timer counts down from.
	uint32_t VALUE; //The actual live countdown value.
	uint32_t CALIB;
}SYSTICK_t;
//function declarations
void BSP_init();
void BSP_led_init();
void BSP_led_Toggle(void);
void BSP_UART_init();
void SysTick_Handler(void)
void BSP_Timer_init(void);
uint32_t BSP_getTick(void);
#endif
