#include <stdint.h>
#include <stm32f10x.h>
#include "ssd1306.h"

int __attribute((noreturn)) main(void) {
    ssd1306_init();
    ssd1306_draw_chessboard();

    while (1) {
        __NOP();
    }
}