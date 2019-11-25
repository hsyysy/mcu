#include <i2c_slave.h>

#define DEFAULT_ISR(name)		\
	void name(void){		\
		while (1)		\
		;			\
	}

// DEFAULT_ISR(isr_systick)
DEFAULT_ISR(isr_hardfault)
DEFAULT_ISR(isr_nmi)
DEFAULT_ISR(isr_svc)
DEFAULT_ISR(isr_pendsv)
DEFAULT_ISR(isr_wwdg)
DEFAULT_ISR(isr_pvd)
DEFAULT_ISR(isr_rtc)
DEFAULT_ISR(isr_flash)
DEFAULT_ISR(isr_rcc)
DEFAULT_ISR(isr_exti0_1)
DEFAULT_ISR(isr_exti2_3)
DEFAULT_ISR(isr_exti4_15)
DEFAULT_ISR(isr_dma1chan1)
DEFAULT_ISR(isr_dma1chan2_3)
DEFAULT_ISR(isr_dma1chan4_7)
DEFAULT_ISR(isr_acd1)
DEFAULT_ISR(isr_lptim1)
DEFAULT_ISR(isr_usart4_5)
DEFAULT_ISR(isr_tim2)
DEFAULT_ISR(isr_tim3)
DEFAULT_ISR(isr_tim6)
DEFAULT_ISR(isr_tim7)
DEFAULT_ISR(isr_tim21)
// DEFAULT_ISR(isr_i2c3)
DEFAULT_ISR(isr_tim22)
// DEFAULT_ISR(isr_i2c1)
DEFAULT_ISR(isr_i2c2)
DEFAULT_ISR(isr_spi1)
DEFAULT_ISR(isr_spi2)
DEFAULT_ISR(isr_usart1)
DEFAULT_ISR(isr_usart2)
DEFAULT_ISR(isr_lpuart1)

extern volatile uint32_t tick;
extern struct i2c_slave_ctx i2c1;
extern struct i2c_slave_ctx i2c3;

void isr_systick(void)
{
	++tick;
}

void isr_i2c3(void)
{
	i2c_slave_isr(&i2c3);
}

void isr_i2c1(void)
{
	i2c_slave_isr(&i2c1);
}
