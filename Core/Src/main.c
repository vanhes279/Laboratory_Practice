#include "../Inc/init.h"
#include "../Inc/it_handlers.h"
#include "../Src/init.c"
// Глобальные переменные
uint16_t flik_list[3] = {2500, 625, 385}; // Периоды для частот 0.2Гц, 0.8Гц, 1.3Гц (в тиках SysTick)
uint8_t current_led_indx = 0;              // Текущий активный светодиод (0-5)
uint8_t mode = 0;                          // 0 - простое свечение, 1 - мерцание
uint8_t BtnCount = 0;                      // Счетчик нажатий кнопки 1
uint8_t shortState = 0;                    // Флаг кратковременного нажатия кнопки 2
uint32_t led_tick = 0;                     // Счетчик времени для мерцания
uint8_t flicker_freq = 0;                  // Текущая частота мерцания (0,1,2)

// Функция выключения всех светодиодов
void OFF(void)
{
    SET_BIT(GPIOE->BSRR, GPIO_BSRR_BR2 | GPIO_BSRR_BR4 | GPIO_BSRR_BR5 | GPIO_BSRR_BR6 | GPIO_BSRR_BR3);
    SET_BIT(GPIOF->BSRR, GPIO_BSRR_BR8);
}

// Функция обновления светодиодов - включает только текущий светодиод (оставляем как было)
void UpdateLEDs(void)
{
    OFF(); // Выключаем все светодиоды
    
    // Включаем только текущий светодиод
    switch (current_led_indx) 
    {
        case 0:
            SET_BIT(GPIOE->BSRR, GPIO_BSRR_BS2);  // PE2
            break;
        case 1:
            SET_BIT(GPIOE->BSRR, GPIO_BSRR_BS4);  // PE4
            break;
        case 2:
            SET_BIT(GPIOE->BSRR, GPIO_BSRR_BS5);  // PE5
            break;
        case 3:
            SET_BIT(GPIOE->BSRR, GPIO_BSRR_BS6);  // PE6
            break;
        case 4:
            SET_BIT(GPIOE->BSRR, GPIO_BSRR_BS3);  // PE3
            break;
        case 5:
            SET_BIT(GPIOF->BSRR, GPIO_BSRR_BS8);  // PF8
            break;           
    }
}

// Функция мерцания для текущего светодиода
void flickers(void)
{
    if (led_tick < flik_list[flicker_freq] / 2)
    {
        // Включаем текущий светодиод на половине периода
        UpdateLEDs();
    }
    else
    {
        // Выключаем все светодиоды
        OFF();
    }
    
    // Сброс счетчика при достижении полного периода
    if (led_tick >= flik_list[flicker_freq])
    {
        led_tick = 0;
    }
}

// Функция изменения частоты мерцания для всех светодиодов
void changeFlickerFrequency(void) 
{
    flicker_freq = (flicker_freq + 1) % 3;
}

// Обработка кнопки 1 - переключение светодиодов
void handleButton1(void)
{
    current_led_indx = (current_led_indx + 1) % 6;
    // Сбрасываем счетчик времени для нового светодиода
    led_tick = 0;
    UpdateLEDs();
}

int main(void)
{
    GPIO_Ini();
    RCC_Ini();
    EXTI_ITR_Ini();
    SysTick_Init();
    
    // Изначально выключаем все светодиоды
    OFF();
    
    while (1)
    {
        // Обработка кнопки 1 (кратковременное нажатие на PC0)
        if (BtnCount > 0)
        {
            handleButton1();
            BtnCount = 0; // Сбрасываем после обработки
        }
        
        // Обработка кнопки 2 - кратковременное нажатие (смена частоты)
        if (shortState)
        {
            changeFlickerFrequency();
            shortState = 0;
            led_tick = 0; // Сбрасываем счетчик при смене частоты
        }
        
        // Основной цикл управления светодиодами
        if (mode) 
        {
            // Режим мерцания
            flickers();
        }
        else 
        {
            // Режим простого свечения
            UpdateLEDs();
        }
    }
}







/*
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
int main(void)
{
   
 GPIO_init_led_pb7_blue ();
 GPIO_init_led_pb14_red ();
 GPIO_init_led_pb0_green ();
 GPIO_button_input();
        while (1)
    {
        
    }
       
      



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

               //UpdateLEDs(); 
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
    }

}


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
       */