#include "./headers/Nokia5110.h"
#include "./headers/TExaS.h"
#include "./headers/GPIO.h"
#include "./headers/XO Game.h"
#include "./headers/Timer.h"
#include "./headers/tm4c123gh6pm.h" 

unsigned int adc_value;    //variable that contains adc value 

int main(void)
{
	TExaS_Init(SSI0_Real_Nokia5110_Scope);

	PortF_Init(); // intialization Port F

	PortB_Init(); // intialization Port B
	
	Nokia5110_Init(); // intialization Nokia Lcd

	Nokia5110_ClearBuffer(); // Clear buffer for nokia lcd

	Nokia5110_DisplayBuffer(); // draw buffer

	GameIntro(); // enter to the game

	GameInitialization(); // reset

	DrawClearGameMatrix(); // clear data
	/* Enable Clock to ADC0 and GPIO pins*/
    SYSCTL_RCGCGPIO_R |= (1<<4);   /* Enable Clock to GPIOE or PE3/AN0 */
	  Timer2(10);									/* 10 msec delay to enable the clock */
    SYSCTL_RCGCADC_R |= (1<<0);    /* ADC0 clock enable*/
    
    /* initialize PE3 for AN0 input  */
    GPIO_PORTE_AFSEL_R |= (1<<3);       /* enable alternate function */
    GPIO_PORTE_DEN_R &= ~(1<<3);        /* disable digital function */
    GPIO_PORTE_AMSEL_R |= (1<<3);       /* enable analog function */
   
    /* initialize sample sequencer3 */
    ADC0_ACTSS_R &= ~(1<<3);        				/* disable SS3 during configuration */
    ADC0_EMUX_R &= ~0xF000;    						/* software trigger conversion */
    ADC0_SSMUX3_R = 0;         	 					/* get input from channel 0 */
    ADC0_SSCTL3_R |= (1<<1)|(1<<2);        /* take one sample at a time, set flag at 1st sample */
    ADC0_ACTSS_R |= (1<<3);         				/* enable ADC0 sequencer 3 */
    
	  /*Iniitialize PF3 as a digital output pin */
		SYSCTL_RCGC2_R |= 0x20;  /* turn on bus clock for GPIOF */
		Timer2(10);							 /* 10 msec delay to enable the clock */
		GPIO_PORTF_DIR_R       |= 0x08;  /* set GREEN pin as a digital output pin */
		GPIO_PORTF_DEN_R       |= 0x08;  /* Enable PF3 pin as a digital pin */ 

	while (1)
	{
		RunGame(); // start game
	ADC0_PSSI_R |= 0x08;           // Start SS3 conversion
        while ((ADC0_RIS_R & 0x08) == 0);  // Wait for conversion to complete
        adc_value = ADC0_SSFIFO3_R;    // Read conversion result
        ADC0_ISC_R = 0x08;             // Clear completion flag

        // Control PF3 (Green LED)
        if (adc_value >= 2048)
            GPIO_PORTF_DATA_R = 0x08;  // Turn on green LED
        else
            GPIO_PORTF_DATA_R = 0x00;  // Turn off green LED

		
	}
}
