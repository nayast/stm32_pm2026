#include "spi.h"

void SPI1_Init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN | RCC_APB2ENR_IOPAEN;

    GPIOA->CRL &= ~((0xFU << (4 * 2)) | (0xFU << (4 * 4)) | (0xFU << (4 * 5)) | (0xFU << (4 * 6)) | (0xFU << (4 * 7)));

    GPIOA->CRL |= (0x3U << 8) | (0x3U << 16) | (0xBU << 20) | (0x3U << 24) | (0xBU << 28);

    GPIOA->ODR |= (1U << 4) | (1U << 2);

    SPI1->CR1 = SPI_CR1_CPOL | SPI_CR1_CPHA | SPI_CR1_MSTR | 
                SPI_CR1_BR | SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_SPE;
}

void SPI1_Write(uint8_t data) {
    while (!(SPI1->SR & SPI_SR_TXE));
    SPI1->DR = data;
}

uint8_t SPI1_Read(void) {
    SPI1->DR = 0;
    while (!(SPI1->SR & SPI_SR_RXNE));
    return SPI1->DR;
}