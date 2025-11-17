#include <stdint.h>
#include <stm32f10x.h>

void delay(uint32_t ticks) {
	for (int i=0; i<ticks; i++) {
		__NOP();
	}
}

/* Interrupt handler */
void TIM2_IRQHandler(void) {
	if (TIM2->SR & TIM_SR_UIF) {
	/* Toggle GPIO here */
	if(GPIOC->ODR & GPIO_ODR_ODR13){
		GPIOC->ODR &= ~GPIO_ODR_ODR13;
	} else {
		GPIOC->ODR |= GPIO_ODR_ODR13;
	}
 	//Clear Interrupt flag
	TIM2->SR &= ~TIM_SR_UIF;
	}
}


int __attribute((noreturn)) main(void) {
	// Enable clock for GPIOC
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPAEN;

	// Enable PC13 push-pull mode
	GPIOC->CRH &= ~(GPIO_CRH_CNF13 | GPIO_CRH_MODE13); //clear cnf13, mode13 bits
	GPIOC->CRH |= GPIO_CRH_MODE13_1; //Max speed = 10Mhz

   	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
   	RCC->APB1RSTR |= RCC_APB1RSTR_TIM2RST;
   	RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM2RST;
   	TIM2->PSC = 719;
   	TIM2->ARR = 25000;
   	TIM2->DIER |= TIM_DIER_UIE; // Enable Update Interrupt
   	NVIC_ClearPendingIRQ(TIM2_IRQn);
   	NVIC_EnableIRQ(TIM2_IRQn); // Enable IRQ in NVIC

   	TIM2->CR1 |= TIM_CR1_CEN; // Start timer

    while (1) {
	    //GPIOC->ODR |= (1U<<13U); //U -- unsigned suffix (to avoid syntax warnings in IDE)
		//GPIOC->ODR |= GPIO_ODR_ODR13;
		//delay(1000000);
	    //GPIOC->ODR &= ~(1U<<13U);
		//GPIOC->ODR &= ~GPIO_ODR_ODR13;
	    //delay(1000000);
    }
}
