#include "../Inc/init.h"
#include "../Inc/it_handlers.h"

//var1
uint16_t flik_list[3] = {5000, 1250, 769}; 
uint8_t current_led_indx = 0;       //текущий светодиод     
uint8_t mode = 0;                   //режим свечения    
uint8_t BtnCount = 0;               //нажатие первой кнопки     
uint8_t shortState = 0;             //короткое нажатие 2 кнопки      
uint32_t led_tick = 0;              //время свечения      
uint8_t flicker_freq = 0;           //индекс для частот     


void OFF(void)
{
    SET_BIT(GPIOE->BSRR, GPIO_BSRR_BR2 | GPIO_BSRR_BR4 | GPIO_BSRR_BR5 | GPIO_BSRR_BR6 | GPIO_BSRR_BR3); //выключам PЕ2,3,4,5,6
    SET_BIT(GPIOF->BSRR, GPIO_BSRR_BR8);                                                                 //выключаем PF8
}

void UpdateLEDs(void)
{
    OFF(); 
    switch (current_led_indx) 
    {
        case 0:
            SET_BIT(GPIOE->BSRR, GPIO_BSRR_BS2);  // вклPE2
            break;
        case 1:
            SET_BIT(GPIOE->BSRR, GPIO_BSRR_BS4);  // вклPE4
            break;
        case 2:
            SET_BIT(GPIOE->BSRR, GPIO_BSRR_BS5);  // вклPE5
            break;
        case 3:
            SET_BIT(GPIOE->BSRR, GPIO_BSRR_BS6);  // вклPE6
            break;
        case 4:
            SET_BIT(GPIOE->BSRR, GPIO_BSRR_BS3);  // вклPE3
            break;
        case 5:
            SET_BIT(GPIOF->BSRR, GPIO_BSRR_BS8);  // вклPF8
            break;           
    }
}

void flickers(void)
{
    if (led_tick < flik_list[flicker_freq] / 2)
    {
        UpdateLEDs();
    }
    else
    {
        OFF();
    }

    if (led_tick >= flik_list[flicker_freq])
    {
        led_tick = 0;
    }
}


void changeFlickerFrequency(void) 
{
    flicker_freq = (flicker_freq + 1) % 3;
}


void handleButton1(void)
{
    current_led_indx = (current_led_indx + 1) % 6;
    led_tick = 0;
}

int main(void)
{
    GPIO_Ini();
    RCC_Ini();
    EXTI_ITR_Ini();
    SysTick_Init();
    
    OFF();
    while (1)
    {
        
        if (BtnCount)
        {
            handleButton1();
            BtnCount = 0; 
        }
        

        if (shortState)
        {
            changeFlickerFrequency();
            shortState = 0;
            led_tick = 0; 
        }
        
        if (mode) 
        {
            OFF();
            flickers();
        }
        else 
        {
            UpdateLEDs();
        }
    }
}