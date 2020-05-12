/*
 * adc.c
 *
 *  Created on: Feb 26, 2020
 *      Author: jschnell
 */
#include "Timer.h"
#include "lcd.h"
#include "uart.h"
#include "math.h"

void adc_init(){
    //Enables clock for port B
    SYSCTL_RCGCGPIO_R |= 0b000010;
    SYSCTL_RCGCADC_R |= 0b1;

    while ((SYSCTL_PRGPIO_R & 0b000010 ) == 0) {};
    while ((SYSCTL_PRADC_R & 0b1 ) == 0) {};

    GPIO_PORTB_DIR_R &= 0b11101111;

    GPIO_PORTB_AFSEL_R |= 0b00010000;

    GPIO_PORTB_DEN_R &= 0b11101111;

    GPIO_PORTB_AMSEL_R |= 0b00010000;

    ADC0_ACTSS_R &= 0b0000;

    ADC0_EMUX_R &= 0b0000;

    ADC0_SSMUX0_R &= 0b0000;
    ADC0_SSMUX0_R |= 0b1010;

    ADC0_SSCTL0_R = 0b0110;

    ADC0_ACTSS_R |= 0b0001;
}

int adc_read(){
    ADC0_PSSI_R |= 0b0001;

    while((ADC0_RIS_R & 0b0001) == 0){}

    return ADC0_SSFIFO0_R;
}


  int main(void) {

    timer_init();
    lcd_init();
    uart_init();
    adc_init();

    while(1){
        double distance = adc_read();

        timer_waitMillis(500);

        double quanVal = distance;

        distance = ((7350000) * pow(distance, -1.7159));

       // distance = 241814 / distance;
      //  distance = distance / 10;


        lcd_printf("%0.2f cm \nQ value: %0.0f\n", distance, quanVal);

        uart_sendChar(distance);
    }

}
