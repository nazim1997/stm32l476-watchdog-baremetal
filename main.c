#define RCC_AHB1_BASE 0x40021000
#define RCC_AHB2_ENR (RCC_AHB1_BASE | 0x4C)
#define AHB2_GPIOA 0x48000000
#define GPIOA_MODER (AHB2_GPIOA)
#define GPIOA_ODR (AHB2_GPIOA | 0x14)

*(volatile uint32_t*) RCC_AHB2_ENR  |= 0x1;
*(volatile uint32_t*) GPIOA_MODER &= ~(0x3 << 10);
*(volatile uint32_t*) GPIOA_MODER |= (0x1 << 10);

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}

int main() {
    while (true) {
        *(volatile uint32_t*) GPIOA_ODR |= (0x1 << 5); // LED ON
        delay(1000);
        *(volatile uint32_t*) GPIOA_ODR &= ~(0x1 << 5); // LED OFF
        delay(1000);
    }
}
