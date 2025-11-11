#include "../Inc/init.h"
#include <stdint.h>
void GPIO_init__Memory(void){};
void GPIO_init_led_pb7_blue (void)
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
// void SysTick_Init(void)
// {
//     CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);
//     SET_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk);
//     SET_BIT(SysTick->CTRL, SysTick_CTRL_CLKSOURCE_Msk);
 //  MODIFY_REG(SysTick->LOAD, SysTick_LOAD_RELOAD_Msk, (180000-1) << SysTick_LOAD_RELOAD_Pos);
//  MODIFY_REG(ysTick->VAL, SysTick_VAL_CURRENT_Msk, (180000-1) << SysTick_VAL_CURRENT_Pos);
// SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);
// }
