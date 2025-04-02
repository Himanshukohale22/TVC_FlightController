/*
@author: Himanshu kohale 
Brief : perticular LED in GPIO port A with pin number 5 will 
        toggle after every 500 ms.

*/

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/systick.h>

/*
Some important keyword 
static - 
extern - 
const - 
volatile - 
*/
volatile uint32_t system_ticks = 0;

static int gpio_setup(void){
    //set clock for GPIO port A 
    rcc_periph_clock_enable(RCC_GPIOA);
    // Set GPIO port 5 
    gpio_set(GPIOA, GPIO_MODE_OUTPUT ,GPIO_PUPD_NONE, GPIO5);
}

//delay function using system_tick sets according to clock
void sys_tick_handler(void){
    system_ticks++;
}
void systick_setup(void) {
    systick_set_reload(84 * 1000);  // 1ms delay (for 84MHz CPU)
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    systick_counter_enable();
    systick_interrupt_enable();
}
void delay_ms(uint32_t ms) {
    uint32_t target = system_ticks + ms;
    while (system_ticks < target);
}


int main(){

    gpio_setup();
    while (1)
    {
        gpio_toggle(GPIOA,GPIO5);
        delay_ms(500); // 500ms
        
    }
    
}