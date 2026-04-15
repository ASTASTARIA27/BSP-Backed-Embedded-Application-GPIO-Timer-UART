#include <stdio.h>
#include <stdint.h>
#include "bsp.h"
#define GPIOD ((GPIO_t*)GPIO_BASE_ADDRESS)
#define RCC ((RCC_t*)RCC_BASE_ADDRESS)
#define USART2 ((USART2_t*)USART2_BASE_ADDRESS)
#define SYSTICK ((SYSTICK_t*)SYSTICK_BASE_ADDRESS)
#define Baudrate  115200
#define CLK_CYCLE 42000000
#define GPIOD_ENABLE         (1<<3)
#define LED_MODER_ENABLE     (1<<24)
#define LED_ODR_ENABLE       (1<<12)
#define RCC_USART2_ENABLE        (1<<17)
#define GPIO_MODER_5_MASK    (3 << 10)
#define GPIO_MODER_6_MASK    (3 << 12)
#define GPIO_MODER_5_ALT     (2 << 10)
#define GPIO_MODER_6_ALT     (2 << 12)
#define ALT_LOW_REG_PIN5     (7<<20)
#define ALT_LOW_REG_PIN6     (7<<24)
#define USART2_CR1_ENABLE        (1<<13)
#define TRANSMIT_ENABLE      (1<<3)
#define RECEIVE_ENABLE       (1<<2)
#define CONTROL_ENABLE       (7<<0)

volatile uint32_t ms_ticks = 0;
void BSP_init() {
	RCC->AHB1ENR |= GPIOD_ENABLE; //enabled the gpiod clock
}
void BSP_led_init() {
	GPIOD->MODER |= LED_MODER_ENABLE; //putting pin12 in output mode
}
void BSP_led_Toggle(void) {
	GPIOD->ODR ^= LED_ODR_ENABLE; //high for pin12 (3.3v)

}
void BSP_UART_init() {
	RCC->APB1ENR |= RCC_USART2_ENABLE; //enabling the clock for USART2
	GPIOD->MODER &=~(GPIO_MODER_5_MASK | GPIO_MODER_6_MASK); //clearing pins
	GPIOD->MODER |=(GPIO_MODER_5_ALT | GPIO_MODER_6_ALT); //setting pin 5 and 6
	GPIOD->AFRL |=(ALT_LOW_REG_PIN5 | ALT_LOW_REG_PIN6); //ALT function low register
	//setting baudrate
	//calculating USART2 DIV
	float USARTDIV = CLK_CYCLE/(16*Baudrate); //16 is the sampling rate check reference manual //not ecactly needed but for reference
	USART2->BRR = 0x16D;
	USART2->CONTROL_REG1 |= USART2_CR1_ENABLE; //enabling the UART
	USART2->CONTROL_REG1 |= TRANSMIT_ENABLE; //transmit enabled
	USART2->CONTROL_REG1 |= RECEIVE_ENABLE;  //receive enable
}

void BSP_Timer_init(void) {
	SYSTICK->LOAD = ((CLK_CYCLE)/1000) -1; //setting the reload value for 1ms interval
	SYSTICK->VALUE = 0; //clearing the current value
	//enable the counter - bit0
	//enable the system exception -bit1
	//use processor clock - bit2
	SYSTICK->CONTROL |= CONTROL_ENABLE;
}

void SysTick_Handler(void) {
	ms_ticks++;
}
uint32_t BSP_getTick(void) {
	return ms_ticks;
}
