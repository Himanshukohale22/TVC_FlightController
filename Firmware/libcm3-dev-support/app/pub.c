/*
@author: Himanshu kohale 
Brief : To check if microcontroller is reading and writing or not 
        which will further used as publisher and subcriber node for
        RTPS communication UART based.

        for this we can use some suitable frameworks like UART/USB/CAN 
        we are using UART here.
        others will probebly use in another firmware

        this is publisher and subcriber will subscribe string from this publisher
        follow "sub.c"

*/

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>


static rcc_setup_pub(){
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_USART2); // use uart according to board 

    // when AF mode is used in GPIO mode it's not a simple GPIO but can be controlled by peripherals(UART,SPI,I2C)

    gpio_mode_setup(GPIOA,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO2); // set GPIO portA pin5 and AF is alternative  function mode
    gpio_set_af(GPIOA,GPIO_AF1,GPIO2);  //set GPIO in alternative function mode
    
    //set UART parameters 

    usart_set_baudrate(USART2,115200);  // set baudrate 
    usart_set_databits(USART2,8);       // 8-bit data will flow as a packet
    usart_set_stopbits(USART2, USART_STOPBITS_1);  // stop bit 
    usart_set_mode(USART2,USART_MODE_TX_RX); //mode tx rx 
    usart_set_parity(USART2,USART_PARITY_NONE);
    usart_set_flow_control(USART2,USART_FLOWCONTROL_NONE);
    usart_enable(USART2);     // lastly enable USART after all set parameters 
    
}

// function for string that will print multiple times.
void uart_send_string(const char *str){
    while (*str)
    {
        usart_send_blocking(USART2, *str++);
    }   

}

int main(){
    rcc_setup_pub();
    while (1)
    {
        uart_send_string("HELLO, STM32 HERE!!");
        for (volatile int i = 0; i < 100000; i++); //simple delay but generally used systick delay method.
  
    }
    
}