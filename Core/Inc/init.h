#include <stdint.h>
void GPIO_init_CMSIS(void);
void GPIO_init__Memory(void);
void GPIO_init_With_Myself_Macros (void);
void GPIO_init_With_Myself_Macros2 (void);

#define RCC_AHB1ENR         *(uint32_t *)(0x40023800UL + 0x30UL)
#define RCC_GPIOB_EN        0x02UL
#define RCC_GPIOC_EN        0x06UL

#define GPIOB_MODER         *(uint32_t *)(0x40020400UL + 0x00UL)
#define GPIOB_OTYPER        *(uint32_t *)(0x40020400UL + 0x04UL)
#define GPIOB_OSPEEDR       *(uint32_t *)(0x40020400UL + 0x08UL)
#define GPIOB_BSRR          *(uint32_t *)(0x40020400UL + 0x18UL)

#define GPIOC_IDR           *(uint32_t *)(0x40020800UL + 0x10UL)

#define GPIO_PIN_MED_14     0x10000000UL
#define GPIO_PIN_OUT_14     0x10000000UL
#define GPIO_PIN_RESET_14   0x40000000UL
#define GPIO_PIN_SET_14     0x4000UL

#define GPIO_PIN_MED_7      0x4000UL
#define GPIO_PIN_OUT_7      0x4000UL
#define GPIO_OFF            0x00UL
#define GPIO_PIN_RESET_7    0x800000UL
#define GPIO_PIN_SET_7      0x80UL
#define GPIO_PIN_13         0x2000UL

#define BIT_SET(REG, BIT)   ((REG) |= (BIT))
#define BIT_READ(REG, BIT)  ((REG) & (BIT))
