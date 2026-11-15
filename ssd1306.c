#include "ssd1306.h"
#include "spi.h"
#include <stm32f10x.h>


#define RES_PIN  2U
#define DC_PIN   6U
#define CS_PIN   4U

static void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms * 8000; i++) {
        __NOP();
    }
}

static void ssd1306_write_cmd(uint8_t cmd) {
    GPIOA->ODR &= ~(1U << CS_PIN);
    GPIOA->ODR &= ~(1U << DC_PIN);
    SPI1_Write(cmd);
    GPIOA->ODR |= (1U << CS_PIN);
}

static void ssd1306_write_data(uint8_t data) {
    GPIOA->ODR &= ~(1U << CS_PIN);
    GPIOA->ODR |= (1U << DC_PIN);
    SPI1_Write(data);
    GPIOA->ODR |= (1U << CS_PIN);
}

void ssd1306_init(void) {
    SPI1_Init();

    GPIOA->ODR &= ~(1U << RES_PIN);
    delay_ms(10);
    GPIOA->ODR |= (1U << RES_PIN);
    delay_ms(10);

    ssd1306_write_cmd(0xAE);
    ssd1306_write_cmd(0xD5);
    ssd1306_write_cmd(0x80);
    ssd1306_write_cmd(0xA8);
    ssd1306_write_cmd(0x3F);
    ssd1306_write_cmd(0xD3);
    ssd1306_write_cmd(0x00);
    ssd1306_write_cmd(0x40);
    ssd1306_write_cmd(0x8D);
    ssd1306_write_cmd(0x14);
    
    ssd1306_write_cmd(0x20); 
    ssd1306_write_cmd(0x00); 

    ssd1306_write_cmd(0xA1);
    ssd1306_write_cmd(0xC8);
    ssd1306_write_cmd(0xDA);
    ssd1306_write_cmd(0x12);
    ssd1306_write_cmd(0x81);
    ssd1306_write_cmd(0xCF);
    ssd1306_write_cmd(0xD9);
    ssd1306_write_cmd(0xF1);
    ssd1306_write_cmd(0xDB);
    ssd1306_write_cmd(0x40);
    ssd1306_write_cmd(0xA4);
    ssd1306_write_cmd(0xA6);
    ssd1306_write_cmd(0xAF);
}

void ssd1306_draw_chessboard(void) {
    ssd1306_write_cmd(0x21);
    ssd1306_write_cmd(0x00);
    ssd1306_write_cmd(0x7F);
    
    ssd1306_write_cmd(0x22);
    ssd1306_write_cmd(0x00);
    ssd1306_write_cmd(0x07);

    for (uint16_t i = 0; i < 1024; i++) {
        uint8_t page = i / 128;
        uint8_t col = i % 128;
        
        if ((page / 1 + col / 8) % 2 == 0) {
            ssd1306_write_data(0xFF);
        } else {
            ssd1306_write_data(0x00);
        }
    }
}