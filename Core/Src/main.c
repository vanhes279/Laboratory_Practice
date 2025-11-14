#include "../Inc/init.h"
#include "../Src/it_handlers.c"
#include "../Src/init.c"








uint8_t flag1 =0;
uint8_t flag2 =0;
uint8_t current_led_indx = 2;
uint8_t led_count = 0;
uint8_t timerbutton1,timerbutton2,led_index = 0;
   
static uint8_t frequency_index = 0;       
const uint32_t frequencies[4] = {1000000, 500000, 250000, 125000};
static uint32_t avtodelay = frequencies[1];
 void UpdateLEDs(void)
    {
        SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR0 |GPIO_BSRR_BR7 | GPIO_BSRR_BR14 );
        
    for (uint8_t i = 0; i < led_count; i++) 
        {
        uint8_t led_index = ((current_led_indx + i) % 3);
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
    // для доп задания
    /*
      void avtoper(uint32_t avtodelay)
        {
            if(led_count == 3)
            {
                SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR0 |GPIO_BSRR_BR7 | GPIO_BSRR_BR14 );
                for(volatile uint32_t i = 0; i < avtodelay; i++){}
                SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS0 |GPIO_BSRR_BS7 | GPIO_BSRR_BS14 );
                for(volatile uint32_t i = 0; i < avtodelay; i++){}
            }
            else{
            current_led_indx = (current_led_indx + 1) % 3;
            for(volatile uint32_t i = 0; i < avtodelay; i++){}
            UpdateLEDs();
            }
        }
            */
int main(void)
{
   
 GPIO_init_led_pb7_blue ();
 GPIO_init_led_pb14_red ();
 GPIO_init_led_pb0_green ();
 GPIO_button_input();
        while (1)
    {
       if (BIT_READ(GPIOC_IDR, GPIO_PIN_8))
       {
        flag1 = 1;
        current_led_indx = (current_led_indx + 1) % 3;
        if(led_count == 2){
            SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR0 |GPIO_BSRR_BR7 | GPIO_BSRR_BR14 );
            for(volatile uint32_t i = 0; i < 100000; i++){}}
        UpdateLEDs();
        for(volatile uint32_t i = 0; i < 1000000; i++);
       }
       else
       {
        flag1 = 0;
       }
       if (BIT_READ(GPIOC_IDR, GPIO_PIN_13))
       {
        flag2 = 1;
        led_count = ((led_count+1))%3;
       
    
        UpdateLEDs();
    
       for(volatile uint32_t i = 0; i < 1000000; i++);
       }
       else
       {
        flag2 = 0;
       }
    }
       
        // дополнительное задание
        /*
         
       {
            timerbutton1++;
            flag1 = 1;
       }
       else
       {
        if(timerbutton1>= 5)
        {
            if (led_count == 3)
            {

               UpdateLEDs(); 
            }
            else
            {
            led_count = ((led_count+1));
            UpdateLEDs();
            }
        }
        if(timerbutton1>= 1 && timerbutton1 < 5)
        {
            if(frequency_index<3){
            frequency_index = (frequency_index+1);
            avtodelay = frequencies[frequency_index];
            }
        }
        
        timerbutton1 = 0;
        flag1 = 0;
       }
       if (BIT_READ(GPIOC_IDR, GPIO_PIN_8))
       {
        timerbutton2++;
        flag2 = 1;
       }
       else
       {
        if(timerbutton2>= 5)
        {
            if (led_count == 0)
            {
               UpdateLEDs(); 
            }
            else
            {
           led_count = ((led_count-1));
            UpdateLEDs();
            }
            
        }
        if(timerbutton2>= 1 && timerbutton2 < 5)
        {
            if(frequency_index>0){
            frequency_index = (frequency_index-1);
            avtodelay = frequencies[frequency_index];
            }
        }
        timerbutton2 = 0;
        flag2 = 0;
       }
       avtoper(avtodelay);
       */
}


