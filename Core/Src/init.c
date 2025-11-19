#include "../Inc/init.h"
#include <stdint.h>

void RCC_Ini(void)
{
/* Предварительная очистка регистров RCC */
MODIFY_REG(RCC->CR, RCC_CR_HSITRIM, 0x80U);
CLEAR_REG(RCC->CFGR);
while(READ_BIT(RCC->CFGR, RCC_CFGR_SWS) != RESET);
CLEAR_BIT(RCC->CR, RCC_CR_PLLON);
while (READ_BIT(RCC->CR, RCC_CR_PLLRDY) != RESET);
CLEAR_BIT(RCC->CR, RCC_CR_HSEON | RCC_CR_CSSON);
while (READ_BIT(RCC->CR, RCC_CR_HSERDY) != RESET);
CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP);
/* Настройка главного регистра RCC */
SET_BIT(RCC->CR, RCC_CR_HSEON);                                                    //Запускаем внешний кварцевыйрезонатор
while(READ_BIT(RCC->CR, RCC_CR_HSERDY) == RESET);                                  //Ждём пока он запустится
CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP);                                                 //Сбросим бит байпаса в 0
SET_BIT(RCC->CR, RCC_CR_CSSON);                                                    //Запустим Clock detector
/* Натсройка регистров PLL
* Предварительная очистка регистра
* Включаем работу PLL*/
CLEAR_REG(RCC->PLLCFGR);
SET_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC_HSE);
MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLM, RCC_PLLCFGR_PLLM_2);
MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLN_Msk, RCC_PLLCFGR_PLLN_2 |RCC_PLLCFGR_PLLN_4 | RCC_PLLCFGR_PLLN_5 | RCC_PLLCFGR_PLLN_7);
CLEAR_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLP_Msk);                                     //Настраивае мпредделитель получившейся частоты после умножения. Иными словами,получаем итоговую частоту PLL
SET_BIT(RCC->CR, RCC_CR_PLLON);                                                    //Запустим PLL
while(READ_BIT(RCC->CR, RCC_CR_PLLRDY));                                           //Ждём запуска PLL84

}

// Функция инициализации GPIO
void GPIO_Ini(void)
{
// Включение тактирования для портов C, E, F
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN |  RCC_AHB1ENR_GPIOEEN | RCC_AHB1ENR_GPIOFEN);

    // PE2
    SET_BIT(GPIOE->MODER, GPIO_MODER_MODE2_0);
    CLEAR_BIT(GPIOE->OTYPER, GPIO_OTYPER_OT_2);
    SET_BIT(GPIOE->OSPEEDR, GPIO_OSPEEDER_OSPEEDR2_0);
    CLEAR_BIT(GPIOE->PUPDR, GPIO_PUPDR_PUPDR2_0);

    // PE3
    SET_BIT(GPIOE->MODER, GPIO_MODER_MODE3_0);
    CLEAR_BIT(GPIOE->OTYPER, GPIO_OTYPER_OT_3);
    SET_BIT(GPIOE->OSPEEDR, GPIO_OSPEEDER_OSPEEDR3_0);
    CLEAR_BIT(GPIOE->PUPDR, GPIO_PUPDR_PUPDR3_0);

    // PE4
    SET_BIT(GPIOE->MODER, GPIO_MODER_MODE4_0);
    CLEAR_BIT(GPIOE->OTYPER, GPIO_OTYPER_OT_4);
    SET_BIT(GPIOE->OSPEEDR, GPIO_OSPEEDER_OSPEEDR4_0);
    CLEAR_BIT(GPIOE->PUPDR, GPIO_PUPDR_PUPDR4_0);

    // PE5
    SET_BIT(GPIOE->MODER, GPIO_MODER_MODE5_0);
    CLEAR_BIT(GPIOE->OTYPER, GPIO_OTYPER_OT_5);
    SET_BIT(GPIOE->OSPEEDR, GPIO_OSPEEDER_OSPEEDR5_0);
    CLEAR_BIT(GPIOE->PUPDR, GPIO_PUPDR_PUPDR5_0);

    // PE6
    SET_BIT(GPIOE->MODER, GPIO_MODER_MODE6_0);
    CLEAR_BIT(GPIOE->OTYPER, GPIO_OTYPER_OT_6);
    SET_BIT(GPIOE->OSPEEDR, GPIO_OSPEEDER_OSPEEDR6_0);
    CLEAR_BIT(GPIOE->PUPDR, GPIO_PUPDR_PUPDR6_0);

    // PE7
    SET_BIT(GPIOE->MODER, GPIO_MODER_MODE7_0);
    CLEAR_BIT(GPIOE->OTYPER, GPIO_OTYPER_OT_7);
    SET_BIT(GPIOE->OSPEEDR, GPIO_OSPEEDER_OSPEEDR7_0);
    CLEAR_BIT(GPIOE->PUPDR, GPIO_PUPDR_PUPDR7_0);

    // PF8
    SET_BIT(GPIOF->MODER, GPIO_MODER_MODE8_0);
    CLEAR_BIT(GPIOF->OTYPER, GPIO_OTYPER_OT_8);
    SET_BIT(GPIOF->OSPEEDR, GPIO_OSPEEDER_OSPEEDR8_0);
    CLEAR_BIT(GPIOF->PUPDR, GPIO_PUPDR_PUPDR8_0);
}

// Функция инициализации внешних прерываний
void EXTI_ITR_Ini(void)
{
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN);                                      // Включение тактирования SYSCFG
// Настройка EXTI0 на линии PC0 (кнопка 1)
    MODIFY_REG(SYSCFG->EXTICR[0], SYSCFG_EXTICR1_EXTI0_Msk, SYSCFG_EXTICR1_EXTI0_PC); //Настройка мультиплексора на вывод линии прерывания EXTI0 на PC0
    SET_BIT(EXTI->IMR, EXTI_IMR_MR0);                                                 //Настройка маскирования 0 линии   
    SET_BIT(EXTI->RTSR, EXTI_RTSR_TR0);                                               //Настройка детектирования нарастающего фронта 0 линии
    SET_BIT(EXTI->FTSR, EXTI_FTSR_TR0);                                               //Настройка детектирования спадающего фронта 0 линии
    
// Настройка EXTI13 на линии PC3 (кнопка 2)
    MODIFY_REG(SYSCFG->EXTICR[0], SYSCFG_EXTICR1_EXTI3_Msk, SYSCFG_EXTICR1_EXTI3_PC); //Настройка мультиплексора на вывод линии прерывания EXTI3 на PC3
    SET_BIT(EXTI->IMR, EXTI_IMR_MR3);                                                 //Настройка маскирования 3 линии
    SET_BIT(EXTI->RTSR, EXTI_RTSR_TR3);                                               //Настройка детектирования нарастающего фронта 3 линии
    SET_BIT(EXTI->FTSR, EXTI_FTSR_TR3);                                               //Настройка детектирования спадающего фронта 3 линии
    

    
// Настройка приоритетов прерываний
    NVIC_SetPriority(EXTI0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));//Установка 0 приоритета прерывания для вектора EXTI0
    NVIC_SetPriority(EXTI3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 0));//Установка 0 приоритета прерывания для вектора EXTI3

    NVIC_EnableIRQ(EXTI0_IRQn);                                                         //Включение прерывания по вектору EXTI0
    NVIC_EnableIRQ(EXTI3_IRQn);                                                         //Включение прерывания по вектору EXTI3
}

// Функция инициализации системного таймера
void SysTick_Init(void)
{
    CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);                                    //На всякий случай предварительно выключим счётчик
    SET_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk);                                     //Разрешаем прерывание по системному таймеру
    SET_BIT(SysTick->CTRL, SysTick_CTRL_CLKSOURCE_Msk);                                   //Источник тактирования будет идти из AHB без деления
    MODIFY_REG(SysTick->LOAD, SysTick_LOAD_RELOAD_Msk,179999 << SysTick_LOAD_RELOAD_Pos); //Будет тикать с частотой 1 кГц и вызывать прерывания
    MODIFY_REG(SysTick->VAL, SysTick_VAL_CURRENT_Msk,179999 << SysTick_VAL_CURRENT_Pos);       //Начнём считать со значения 0
    SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);                                      //Включим счётчик
}