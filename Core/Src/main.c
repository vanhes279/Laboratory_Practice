#include "../Inc/init.h"
#include "../Src/init.c"
#include <stdint.h>


int main(void)
{
    GPIO_init_With_Myself_Macros ();
    GPIO_init_With_Myself_Macros2 ();
    while (1)
    {
        
       
       
        if (BIT_READ(GPIOC_IDR, GPIO_PIN_13))
        {
            BIT_SET(GPIOB_BSRR, GPIO_PIN_SET_7);   
            BIT_SET(GPIOB_BSRR, GPIO_PIN_RESET_14);   
        }
        else
        {
            BIT_SET(GPIOB_BSRR, GPIO_PIN_SET_14);
            BIT_SET(GPIOB_BSRR, GPIO_PIN_RESET_7);
        }
    }
}