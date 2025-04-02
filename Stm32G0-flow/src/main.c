// @author: Himanshu kohale 
// Brief : about this file 

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

static void gpio_setup(void)
{
    // Enable GPIOA clock
    rcc_periph_clock_enable(RCC_GPIOA);
    
    // Set GPIO5 (LED on Nucleo-G071RB) to output push-pull
    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO5);
}

int main(void)
{
    gpio_setup();
    
    // Blink LED
    while (1) {
        gpio_toggle(GPIOA, GPIO5);
        for (int i = 0; i < 1000000; i++) __asm__("nop");
    }
    
    return 0;
}