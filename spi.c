#include "spi.h"

void SPI1_Init(void) {
    // 1. включение тактирования SPI1 и GPIOA
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN | RCC_APB2ENR_IOPAEN;

    // 2. настройка пинов GPIOA:
    // PA5 (SCK) и PA7 (MOSI) - функция push-pull (cnf = 10, mode = 11 -> 0xB)
    // PA4 (CS), PA2 (RES), PA6 (DC) - обычный выход push-pull (cnf = 00, mode = 11 -> 0x3)
    
    // очищаем конфигурацию для PA2, PA4, PA5, PA6, PA7 в CRL
    GPIOA->CRL &= ~((0xFU << (4 * 2)) | (0xFU << (4 * 4)) | (0xFU << (4 * 5)) | (0xFU << (4 * 6)) | (0xFU << (4 * 7)));

    // PA2: mode=11, cnf=00 -> 0x3 << 8
    // PA4: mode=11, cnf=00 -> 0x3 << 16
    // PA5: mode=11, cnf=10 -> 0xB << 20
    // PA6: mode=11, cnf=00 -> 0x3 << 24
    // PA7: mode=11, cnf=10 -> 0xB << 28
    GPIOA->CRL |= (0x3U << 8) | (0x3U << 16) | (0xBU << 20) | (0x3U << 24) | (0xBU << 28);

    // начальные состояния управляющих пинов: CS=1, RES=1
    GPIOA->ODR |= (1U << 4) | (1U << 2);

    // 3. конфигурирование SPI1
    // CPOL = 1, CPHA = 1, MSTR = 1, BR = 111 (f_PCLK/256), SSM = 1, SSI = 1, SPE = 1
    SPI1->CR1 = SPI_CR1_CPOL | SPI_CR1_CPHA | SPI_CR1_MSTR | 
                SPI_CR1_BR | SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_SPE;
}

void SPI1_Write(uint8_t data) {
    // ждем, пока буфер передатчика не освободится (TXE = 1)
    while (!(SPI1->SR & SPI_SR_TXE));
    // записываем данные в буфер
    SPI1->DR = data;
    // ждем окончания передачи
    while (SPI1->SR & SPI_SR_BSY);
}

uint8_t SPI1_Read(void) {
    SPI1->DR = 0; // Запускаем обмен
    while (!(SPI1->SR & SPI_SR_RXNE)); // ждем приема
    return SPI1->DR;
}