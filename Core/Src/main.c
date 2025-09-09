#include <stdint.h>

int main(void){

    *(uint32_t*)(0x40023800 + 0x30) |= 0x02;




    while(1){}
}