#include <stdint.h>
#include <stm32f10x.h>

void delay(uint32_t ticks) {
	for (int i=0; i<ticks; i++) {
		__NOP();
	}
}

uint8_t SPI1_Read(void)
{
 SPI1->DR = 0; //запускаем обмен
  //Ждем, пока не появится новое значение
 //в буфере приемника
 while(!(SPI1->SR & SPI_SR_RXNE));
  //возвращаем значение буфера приемника
 return SPI1->DR;
}

void SPI1_Write(uint8_t data)
{
 //Ждем, пока не освободится буфер передатчика
 while(!(SPI1->SR & SPI_SR_TXE));
  //заполняем буфер передатчика
 SPI1->DR = data;
}

void display_cmd(uint8_t cmd){
	GPIOA->ODR &= ~GPIO_ODR_ODR4; // CS=0
	GPIOA->ODR &= ~GPIO_ODR_ODR1; // DC=0
	delay(1000);
	SPI1_write(cmd);
	GPIOA->ODR |= GPIO_ODR_ODR4; // CS=1
}

int __attribute((noreturn)) main(void) {
	// Enable clock for GPIOC
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPAEN;
	// Enable clock for SPI1
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

	// Configure SPI
	SPI1->CR1 |= SPI_CR1_CPOL | SPI_CR1_CPHA |
				SPI_CR1_MSTR | SPI_CR1_BR |
				SPI_CR1_SSM | SPI_CR1_SPE;


	// Enable PC13 push-pull mode
	GPIOC->CRH &= ~(GPIO_CRH_CNF13 | GPIO_CRH_MODE13); //clear cnf13, mode13 bits
	GPIOC->CRH |= GPIO_CRH_MODE13_1; //Max speed = 10Mhz

	// Configure SPI Pins
	GPIOA->CRL &= ~(GPIO_CRL_CNF2 | GPIO_CRL_MODE2);
	GPIOA->CRL 

	// Configure Display Pins
	uint8_t display_on = 1; // 0 or 1
	display_cmd(0xAE | display_on);

    while (1) {
	    //GPIOC->ODR |= (1U<<13U); //U -- unsigned suffix (to avoid syntax warnings in IDE)
		GPIOC->ODR |= GPIO_ODR_ODR13;
		delay(1000000);
	    //GPIOC->ODR &= ~(1U<<13U);
		GPIOC->ODR &= ~GPIO_ODR_ODR13;
	    delay(1000000);
    }
}
