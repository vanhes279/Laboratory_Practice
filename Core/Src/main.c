#include <stdint.h>

int main(void)
{

    *(uint32_t *)(0x40023800UL + 0x30UL) |= 0x02UL;   // вкл тактирования GPIOB
    *(uint32_t *)(0x40020400UL + 0x00UL) |= 0x4000UL; // настройка режима пина PB7
    *(uint32_t *)(0x40020400UL + 0x04UL) = 0x00UL;    // зануление
    *(uint32_t *)(0x40020400UL + 0x08UL) |= 0x4000UL; // настройка регистра ospeedr

    *(uint32_t *)(0x40020400UL + 0x18UL) |= 0x80UL;   // vkl svetodiod

    while (1)
    {
    }
}