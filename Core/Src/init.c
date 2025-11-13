#include "../Inc/init.h"
#include <stdint.h>
void GPIO_init__Memory(void){};


// Функция инициализации тактирования системы
void RCC_Ini(void)
{
    /* Предварительная очистка регистров RCC */
    MODIFY_REG(RCC->CR, RCC_CR_HSITRIM, 0x80U);  // Установка калибровочного значения для HSI
    CLEAR_REG(RCC->CFGR);                        // Сброс регистра конфигурации тактирования
    while (READ_BIT(RCC->CFGR, RCC_CFGR_SWS) != RESET);  // Ожидание сброса источника системного такта
    CLEAR_BIT(RCC->CR, RCC_CR_PLLON);           // Выключение PLL
    while (READ_BIT(RCC->CR, RCC_CR_PLLRDY) != RESET);   // Ожидание готовности PLL к выключению
    CLEAR_BIT(RCC->CR, RCC_CR_HSEON | RCC_CR_CSSON);     // Выключение HSE и детектора тактирования
    while (READ_BIT(RCC->CR, RCC_CR_HSERDY) != RESET);   // Ожидание выключения HSE
    CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP);          // Сброс байпаса HSE (используем кварц)

    /* Настройка главного регистра RCC */
    SET_BIT(RCC->CR, RCC_CR_HSEON);             // Включение внешнего кварцевого резонатора
    while (READ_BIT(RCC->CR, RCC_CR_HSERDY) == RESET);   //Ждём пока он запустится
    CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP);          //Сбросим бит байпаса в 0, если вдруг тамчто-то лежит
    SET_BIT(RCC->CR, RCC_CR_CSSON);             // Включаем clock detector

    CLEAR_REG(RCC->PLLCFGR);                    // Сброс регистра конфигурации PLL
    SET_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC_HSE);  // Выбор HSE как источника для PLL
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLM, RCC_PLLCFGR_PLLM_2); // Делитель PLLM = 4
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLN_Msk, RCC_PLLCFGR_PLLN_2 |
    RCC_PLLCFGR_PLLN_4 | RCC_PLLCFGR_PLLN_5 | RCC_PLLCFGR_PLLN_7); // Умножение PLLN = 168
    CLEAR_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLP_Msk);     // Делитель PLLP = 2 (для получения 84 МГц)
    SET_BIT(RCC->CR, RCC_CR_PLLON);             // Включение PLL
    while (READ_BIT(RCC->CR, RCC_CR_PLLRDY));   // Ожидание готовности PLL

    MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL);  // Выбор PLL как системного такта
    MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_CFGR_HPRE_DIV1);  // AHB без деления (84 МГц)
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_CFGR_PPRE1_DIV4); // APB1 делитель на 4 (42 МГц)
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, RCC_CFGR_PPRE2_DIV2); // APB2 делитель на 2 (84 МГц)
    MODIFY_REG(RCC->CFGR, RCC_CFGR_MCO2PRE, RCC_CFGR_MCO2PRE_Msk); // Делитель для MCO2
    CLEAR_BIT(RCC->CFGR, RCC_CFGR_MCO2);        // Настройка вывода MCO2

    MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_5WS);  // Задержка доступа к Flash
}

// Функция инициализации GPIO
void GPIO_Ini(void)
{
    // Включение тактирования для портов B, C, E, F
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN |
            RCC_AHB1ENR_GPIOEEN | RCC_AHB1ENR_GPIOFEN);

    // Настройка вывода PC13 (кнопка) как вход с подтяжкой к земле
    SET_BIT(GPIOC->PUPDR, GPIO_PUPDR_PUPD13_1);

    // Настройка выводов как выходов Push-Pull с высокой скоростью и подтяжками:
    
    // PB0 (зеленый светодиод)
    SET_BIT(GPIOB->MODER, GPIO_MODER_MODE0_0);  // Режим выхода
    CLEAR_BIT(GPIOB->OTYPER, GPIO_OTYPER_OT_0); // Push-Pull
    SET_BIT(GPIOB->OSPEEDR, GPIO_OSPEEDER_OSPEEDR0_0); // Средняя скорость
    SET_BIT(GPIOB->PUPDR, GPIO_PUPDR_PUPDR0_0); // Подтяжка к питанию

    // PB7 (синий светодиод)
    SET_BIT(GPIOB->MODER, GPIO_MODER_MODE7_0);
    CLEAR_BIT(GPIOB->OTYPER, GPIO_OTYPER_OT_7);
    SET_BIT(GPIOB->OSPEEDR, GPIO_OSPEEDER_OSPEEDR7_0);
    CLEAR_BIT(GPIOB->PUPDR, GPIO_PUPDR_PUPDR7_0); // Без подтяжки

    // PB14 (красный светодиод)
    SET_BIT(GPIOB->MODER, GPIO_MODER_MODE14_0);
    CLEAR_BIT(GPIOB->OTYPER, GPIO_OTYPER_OT_14);
    SET_BIT(GPIOB->OSPEEDR, GPIO_OSPEEDER_OSPEEDR14_0);
    CLEAR_BIT(GPIOB->PUPDR, GPIO_PUPDR_PUPDR14_0); // Без подтяжки

    // Дополнительные пины на портах E и F (вероятно, для периферии)
    // PE2
    SET_BIT(GPIOE->MODER, GPIO_MODER_MODE2_0);
    CLEAR_BIT(GPIOE->OTYPER, GPIO_OTYPER_OT_2);
    SET_BIT(GPIOE->OSPEEDR, GPIO_OSPEEDER_OSPEEDR2_0);
    CLEAR_BIT(GPIOE->PUPDR, GPIO_PUPDR_PUPDR2_0);

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
    SET_BIT(GPIOE->MODER, GPIO_MODER_MODE3_0);
    CLEAR_BIT(GPIOE->OTYPER, GPIO_OTYPER_OT_3);
    SET_BIT(GPIOE->OSPEEDR, GPIO_OSPEEDER_OSPEEDR3_0);
    CLEAR_BIT(GPIOE->PUPDR, GPIO_PUPDR_PUPDR3_0);

    // PF8
    SET_BIT(GPIOF->MODER, GPIO_MODER_MODE8_0);
    CLEAR_BIT(GPIOF->OTYPER, GPIO_OTYPER_OT_8);
    SET_BIT(GPIOF->OSPEEDR, GPIO_OSPEEDER_OSPEEDR8_0);
    CLEAR_BIT(GPIOF->PUPDR, GPIO_PUPDR_PUPDR8_0);
}

// Функция инициализации внешних прерываний
void EXTI_ITR_Ini(void)
{
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN);
    
    // Настройка EXTI0 на линии PC0 (кнопка 1)
    MODIFY_REG(SYSCFG->EXTICR[0], SYSCFG_EXTICR1_EXTI0_Msk, SYSCFG_EXTICR1_EXTI0_PC);
    SET_BIT(EXTI->IMR, EXTI_IMR_MR0);
    SET_BIT(EXTI->RTSR, EXTI_RTSR_TR0);
    SET_BIT(EXTI->FTSR, EXTI_FTSR_TR0);
    
    // Настройка EXTI13 на линии PC13 (кнопка 2)
    MODIFY_REG(SYSCFG->EXTICR[3], SYSCFG_EXTICR4_EXTI13_Msk, SYSCFG_EXTICR4_EXTI13_PC);
    SET_BIT(EXTI->IMR, EXTI_IMR_MR13);
    SET_BIT(EXTI->RTSR, EXTI_RTSR_TR13);
    SET_BIT(EXTI->FTSR, EXTI_FTSR_TR13);
    
    // Настройка приоритетов прерываний
    NVIC_SetPriority(EXTI0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    NVIC_SetPriority(EXTI15_10_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 0));
    
    NVIC_EnableIRQ(EXTI0_IRQn);
    NVIC_EnableIRQ(EXTI15_10_IRQn);
}

// Функция инициализации системного таймера
void SysTick_Init(void)
{
    CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);  // Временное отключение таймера
    SET_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk);   // Разрешение прерываний
    SET_BIT(SysTick->CTRL, SysTick_CTRL_CLKSOURCE_Msk); // Тактирование от процессорной шины
    // Установка периода перезагрузки для генерации 1 кГц (при 180 МГц системной частоты)
    MODIFY_REG(SysTick->LOAD, SysTick_LOAD_RELOAD_Msk, 179999 << SysTick_LOAD_RELOAD_Pos);
    // Установка начального значения счетчика
    MODIFY_REG(SysTick->VAL, SysTick_VAL_CURRENT_Msk, 179999 << SysTick_VAL_CURRENT_Pos);
    SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);    // Запуск таймера
}
/*void GPIO_init_led_pb7_blue (void)
{
*(uint32_t *)(0x40023800UL + 0x30UL) |= 0x02UL;
*(uint32_t *)(0x40023800UL + 0x30UL) |= 0x06UL;
*(uint32_t *)(0x40020400UL + 0x00UL) |= 0x4000UL;
*(uint32_t *)(0x40020400UL + 0x04UL) |= 0x00UL;
*(uint32_t *)(0x40020400UL + 0x08UL) |= 0x4000UL;
*(uint32_t *)(0x40020400UL + 0x18UL) |= 0x800000UL;
}
void GPIO_init_led_pb14_red (void)
{
    RCC_AHB1ENR |= RCC_GPIOB_EN | RCC_GPIOC_EN;
    BIT_SET(GPIOB_MODER, GPIO_PIN_OUT_14);
    BIT_SET(GPIOB_OTYPER, GPIO_OFF);
    BIT_SET(GPIOB_OSPEEDR, GPIO_PIN_MED_14);
    BIT_SET(GPIOB_BSRR, GPIO_PIN_RESET_14);
}
void GPIO_init_led_pb0_green (void)
{
   SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN);
   SET_BIT(GPIOB->MODER, GPIO_MODER_MODE0_0);
   CLEAR_BIT(GPIOB->OTYPER,GPIO_OTYPER_OT_0);
   SET_BIT(GPIOB->OSPEEDR, GPIO_OSPEEDER_OSPEEDR0_0);
   SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR0);
}
void GPIO_button_input(void)
{
    //PС12
    CLEAR_BIT(GPIOB->MODER, GPIO_MODER_MODE10_0);
    CLEAR_BIT(GPIOB->PUPDR, GPIO_PUPDR_PUPDR10_0);
}
void SysTick_Init(void)
{
    CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);
    SET_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk);
    SET_BIT(SysTick->CTRL, SysTick_CTRL_CLKSOURCE_Msk);
    MODIFY_REG(SysTick->LOAD, SysTick_LOAD_RELOAD_Msk, (180000-1) << SysTick_LOAD_RELOAD_Pos);
    MODIFY_REG(SysTick->VAL, SysTick_VAL_CURRENT_Msk, (180000-1) << SysTick_VAL_CURRENT_Pos);
    SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);
}
*/