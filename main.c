#include <stdint.h>
#include <stdbool.h>

#define STK_CTRL 0xE000E010
#define STK_LOAD 0xE000E014
#define STK_VAL  0xE000E018

#define RCC_AHB1_BASE 0x40021000
#define RCC_AHB2_ENR (RCC_AHB1_BASE | 0x4C)
#define AHB2_GPIOA 0x48000000
#define GPIOA_MODER (AHB2_GPIOA)
#define GPIOA_ODR (AHB2_GPIOA | 0x14)

#define IWDG_BASE 0x40003000
#define IWDG_KR (IWDG_BASE)


void systick_init() {
    // Enable Counter
    *(volatile uint32_t*) STK_CTRL |= 0x1;
    // Select Processor Clock
    *(volatile uint32_t*) STK_CTRL |= (0x1 << 2);
}

void delay_ms(volatile uint32_t count) {
    if (count < 4194) {
        count = count * 0xFA0;
    }
    else {
        count = 0xFFFFFF;
    }
    *(volatile uint32_t*) STK_LOAD = count;
    *(volatile uint32_t*) STK_VAL = 0x0;
    while (!((*(volatile uint32_t*) STK_CTRL) & (1 << 16)));
}

void iwdg_init() {
    *(volatile uint32_t*) IWDG_KR = 0x0000CCCC;
}

void iwdg_feed() {
    *(volatile uint32_t*) IWDG_KR = 0x0000AAAA;
}

void gpio_init() {
    *(volatile uint32_t*) RCC_AHB2_ENR  |= 0x1;
    *(volatile uint32_t*) GPIOA_MODER &= ~(0x3 << 10);
    *(volatile uint32_t*) GPIOA_MODER |= (0x1 << 10);
}

void led_blink() {
    *(volatile uint32_t*) GPIOA_ODR |= (0x1 << 5); // LED ON
    delay_ms(500);
    iwdg_feed();
    *(volatile uint32_t*) GPIOA_ODR &= ~(0x1 << 5); // LED OFF
    delay_ms(500);
    iwdg_feed();
}


int main() {
    systick_init();

    gpio_init();

    iwdg_init();

    while (true) {
        led_blink();
    }
}
