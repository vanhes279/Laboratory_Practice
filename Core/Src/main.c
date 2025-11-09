#include "../Inc/init.h"
#include "../Src/init.c"
#include <stdint.h>



uint8_t current_led_indx = 0;
uint8_t led_count = 0;
uint8_t led_index = 0;
int main(void)
{
    void UpdateLEDs(void)
    {
        SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR0 |GPIO_BSRR_BR7 | GPIO_BSRR_BR14 );

    for (uint8_t i = 0; i < led_count; i++) 
        {
        uint8_t led_index = (current_led_indx + i) % 3;
        switch (led_index) 
            {
            case 0:
                SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS0);  // PB0
                break;
            case 1:
                SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS7);  // PB7
                break;
            case 2:
                SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS14); // PB14
                break;
            }
        }
    }
 GPIO_init_led_pb7_blue ();
 GPIO_init_led_pb14_red ();
 GPIO_init_led_pb0_green ();
 GPIO_button_input();
    while (1)
    {
       if (BIT_READ(GPIOC_IDR, GPIO_PIN_13))
       {
        current_led_indx = (current_led_indx + 1) % 3;
        UpdateLEDs();
       }
       if (BIT_READ(GPIOC_IDR, GPIO_PIN_12))
       {
        led_count = (led_count % 3) + 1;
        UpdateLEDs();
       }
       
    }
}






























// if (BIT_READ(GPIOC_IDR, GPIO_PIN_13))
//         {
//             BIT_SET(GPIOB_BSRR, GPIO_PIN_SET_7);   
//             BIT_SET(GPIOB_BSRR, GPIO_PIN_RESET_14);   
//         }