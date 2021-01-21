#include "stm32f3xx.h"                  // Device header

void init_led(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOEEN;
	
	GPIOE -> MODER |= 0x55550000;
}

void TIM6_enable(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
	
	TIM6->CR1 |= TIM_CR1_OPM;
	
}
/* Delay function using system clock */
/* SR, the status register. */
/* EGR, the event generation register. */
/* CNT, the counter register. */
/* PSC, the prescaler register. */
/* ARR, the autoreload register. */
void delay(uint16_t ms)
{
	/* Set PSC frequency */
	TIM6->PSC = (uint16_t)7999;
	/* Value to reach */
	TIM6->ARR = ms;
	/* enable clock */
	TIM6->CR1 |= (uint16_t)1;
	/* wait event signaling that counter has reach value */
	while (TIM6->SR == 0);
	/* Reset event */
	TIM6->SR = 0;
}

//********7-SEG********
void init_gpio_shd0028(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	//LATCH  PC1  
	GPIOC -> MODER |= GPIO_MODER_MODER1_0;
	//ENABLE PC3
	GPIOC -> MODER |= GPIO_MODER_MODER3_0;
}

void init_spi_shd0028(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	//PA5 - SCL  -  Alternative func. push-pull - OUT 			
	//PA6 - MISO -  Alternative func. push-pull - OUT  - IN     
	//PA7 - MOSI -  Alternative func.  push-pull - OUT      
	
	GPIOA->MODER |= GPIO_MODER_MODER5_1 | GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1;
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5 | GPIO_OSPEEDER_OSPEEDR7;
	GPIOA->PUPDR &=~(GPIO_PUPDR_PUPDR5 | GPIO_PUPDR_PUPDR7);
	
	GPIOA->AFR[0] |= 0x55500000;
	
	//SPI config
	SPI1->CR1 |= SPI_CR1_BR;
	SPI1->CR1 |= SPI_CR1_CPOL;
	SPI1->CR1 |= SPI_CR1_CPHA;
	SPI1->CR1 &= ~SPI_CR1_LSBFIRST;
	SPI1->CR1 |= SPI_CR1_SSM;
	SPI1->CR1 |= SPI_CR1_SSI;
	SPI1->CR1 |= SPI_CR1_MSTR;
	SPI1->CR1 |= SPI_CR1_SPE;
	
	
}

uint16_t Spi_Write_Data(uint32_t data)
{
  	GPIOC->BSRR |= GPIO_BSRR_BR_1;
		GPIOC->BSRR |= GPIO_BSRR_BS_3;
		
		
		
	//GPIOC->BSRR |= GPIO_BSRR_BR_3;
	while(!(SPI1->SR & SPI_SR_TXE));
	
	//CS_LOW   
        
	SPI1->DR = data;  

       
	while(!(SPI1->SR & SPI_SR_RXNE));
	GPIOC->BSRR |= GPIO_BSRR_BS_1;
	delay(2);
	GPIOC->BSRR |= GPIO_BSRR_BR_1;		
	delay(2);
	GPIOC->BSRR |= GPIO_BSRR_BR_3;
	
  //GPIOC->BSRR |= GPIO_BSRR_BS_1;
	
	data = SPI1->DR;  
	//GPIOC->BSRR |= GPIO_BSRR_BR_1;
	//GPIOC->BSRR |= GPIO_BSRR_BS_3;
	//CS_HIGH   
        
       return data;  
}

int main (void)
{
	TIM6_enable();
	
	init_led();

	init_gpio_shd0028();
	init_spi_shd0028();
	
	uint16_t num1[10] = {0x3f,0x6,0x5b,0x4f,0x66,0x6d,0x7d,0x7,0x7f,0x6f};
	uint16_t num2[10] = {0x3f00,0x600,0x5b00,0x4f00,0x6600,0x6d00,0x7d00,0x700,0x7f00,0x6f00};
	uint8_t i,j,k,u;
	
	
	while(1)
	{
		
		k=100;
		i=0;
		//j=0;
		while (k>0) 
		{
			if (i==10) i=0;
			j=0;
			u=10;
			while(u>0)
			{
				if (j==10) j=0;
				Spi_Write_Data(num1[j]+num2[i]);
				//GPIOC->BSRR |= GPIO_BSRR_BS_3;	
				GPIOE->BSRR |= GPIO_BSRR_BS_10;		
				delay(50);
				//Spi_Write_Data(num1[9]+num2[9]);
				GPIOE->BSRR |= GPIO_BSRR_BR_10;
				//Spi_Write_Data(0x55);
				delay(50);
				j++;
				u--;
				
			}
			i++;
			k--;
			
		}

	}
}


