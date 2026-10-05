#include <stdint.h>
#include <stdbool.h>

#define RCC_AHB1_BASE 0x40021000
#define RCC_AHB2_ENR (RCC_AHB1_BASE | 0x4C)
#define AHB2_GPIOA 0x48000000
#define GPIOA_MODER (AHB2_GPIOA)
#define GPIOA_ODR (AHB2_GPIOA | 0x14)

#define IWDG_BASE 0x40003000
#define IWDG_KR (IWDG_BASE)


void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
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
    delay(50000);
    *(volatile uint32_t*) GPIOA_ODR &= ~(0x1 << 5); // LED OFF
    delay(50000);
}


int main() {
    gpio_init();

    iwdg_init();

    while (true) {
        led_blink();
    }
}
