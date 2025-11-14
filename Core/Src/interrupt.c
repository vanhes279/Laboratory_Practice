#include <stdint.h>
uint32_t delay_count;
uint8_t FLAG_DELAY;
uint32_t global_systick_tim_count;
void SysTick_Handler(void)
{
    global_systick_tim_count++;
    if(FLAG_DELAY){
delay_count++;
    }
}
extern volatile uint16_t button_dellay_tim_count;
void delay(uint32_t del)
{
    FLAG_DELAY=1;
    while(del >= delay_count){}
    delay_count = 0;
    FLAG_DELAY=0;
}