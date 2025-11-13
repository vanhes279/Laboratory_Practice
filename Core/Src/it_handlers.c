#include "../Inc/it_handlers.h"

extern uint32_t led_tick;
extern uint8_t BtnCount;
extern uint8_t shortState;
extern uint8_t mode;
extern uint8_t flicker_freq;

uint32_t ExternalInterruptTickCount = 0;
uint8_t btn1State = 0;     // Состояние кнопки 1 (PC0)
uint8_t btn2State = 0;     // Состояние кнопки 2 (PC13)  
uint32_t button1PressTime = 0;
uint32_t button2PressTime = 0;

void SysTick_Handler(void)
{
    ExternalInterruptTickCount++;
    led_tick++; // Увеличиваем счетчик для мерцания
}

void EXTI0_IRQHandler(void)
{
    SET_BIT(EXTI->PR, EXTI_PR_PR0); // Сброс флага прерывания
    
    if (ExternalInterruptTickCount >= DELAY_BUTTON_FILTER)
    {
        // Обработка кнопки 1 (PC0) - простое нажатие
        BtnCount++;
        ExternalInterruptTickCount = 0;
    }
}

// Добавляем обработчик для кнопки 2 (PC13)
void EXTI15_10_IRQHandler(void)
{
    if (READ_BIT(EXTI->PR, EXTI_PR_PR13)) 
    {
        SET_BIT(EXTI->PR, EXTI_PR_PR13); // Сброс флага прерывания
        
        if (ExternalInterruptTickCount >= DELAY_BUTTON_FILTER)
        {
            if (btn2State == 0)
            {
                // Первое нажатие - начинаем отсчет времени
                button2PressTime = ExternalInterruptTickCount;
                btn2State = 1;
            }
            else
            {
                // Отпускание кнопки - определяем тип нажатия
                uint32_t holdTime = ExternalInterruptTickCount - button2PressTime;
                
                if (holdTime > 2000) 
                {
                    // Удержание более 2 секунд - смена режима мерцание/свечение
                    mode = !mode;
                    led_tick = 0; // Сбрасываем счетчик при смене режима
                }
                else if (holdTime > 100 && holdTime <= 2000)
                {
                    // Кратковременное нажатие - смена частоты мерцания
                    shortState = 1;
                }
                
                btn2State = 0;
                ExternalInterruptTickCount = 0;
            }
        }
    }
}