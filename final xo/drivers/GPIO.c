#include "..\\./headers/GPIO.h"
#include "..\\./headers/tm4c123gh6pm.h"
#include "..\\./headers/TExaS.h"
#include "..\\./headers/Timer.h"
#include "../TM4C123GH6PM.h"
GPIOA_Type* gpioPortF = (GPIOA_Type*) GPIOF_BASE;
volatile unsigned long delay;
unsigned char Sw1, Sw2;

void PortB_Init(void)
{
  SYSCTL_RCGC2_R |= 0x00000002;   // 1) B clock
  delay = SYSCTL_RCGCGPIO_R;      // delay
  GPIO_PORTB_LOCK_R = 0x4C4F434B; // 2)unlock GPIO of PORTB
  GPIO_PORTB_CR_R = 0x01;         // Enable commit
  GPIO_PORTB_AMSEL_R = 0x00;      // 3) disable analog function
  GPIO_PORTB_PCTL_R = 0x00000000; // 4) GPIO clear bit PCTL
  GPIO_PORTB_DIR_R = 0xFF;        // 5) PORT output
  GPIO_PORTB_AFSEL_R = 0x00;      // 6) no alternate function
  GPIO_PORTB_DEN_R = 0xFF;        // 7) enable digital pins PF4-PF0
}

// Predefined configuration for Port F
static const GpioF_PinConfig_t portFConfigs[] = {
    {GPIOF_PIN_0, GPIOF_DIR_INPUT, GPIOF_PUR_ENABLE, GPIOF_DEN_ENABLE, GPIO_AFSEL_DISABLE, GPIO_AMSEL_DISABLE, GPIO_PCTL_GPIO, GPIOF_EDGE_SENSITIVE, GPIOF_ONE_EDGE, GPIOF_FALLING_EDGE, GPIOF_ICR_CLEAR, GPIOF_IM_ENABLE},  // SW2 (PF0)
    {GPIOF_PIN_1, GPIOF_DIR_OUTPUT, GPIOF_PUR_DISABLE, GPIOF_DEN_ENABLE, GPIO_AFSEL_DISABLE, GPIO_AMSEL_DISABLE, GPIO_PCTL_GPIO}, // Red LED
    {GPIOF_PIN_2, GPIOF_DIR_OUTPUT, GPIOF_PUR_DISABLE, GPIOF_DEN_ENABLE, GPIO_AFSEL_DISABLE, GPIO_AMSEL_DISABLE, GPIO_PCTL_GPIO}, // Blue LED
    {GPIOF_PIN_3, GPIOF_DIR_OUTPUT, GPIOF_PUR_DISABLE, GPIOF_DEN_ENABLE, GPIO_AFSEL_DISABLE, GPIO_AMSEL_DISABLE, GPIO_PCTL_GPIO}, // Green LED
    {GPIOF_PIN_4, GPIOF_DIR_INPUT, GPIOF_PUR_ENABLE, GPIOF_DEN_ENABLE, GPIO_AFSEL_DISABLE, GPIO_AMSEL_DISABLE, GPIO_PCTL_GPIO, GPIOF_EDGE_SENSITIVE, GPIOF_ONE_EDGE, GPIOF_FALLING_EDGE, GPIOF_ICR_CLEAR, GPIOF_IM_ENABLE}   // SW1 (PF4)
};

void PortF_Init(void) {
    volatile unsigned long delay;
		int pin;
    // Enable clock for Port F
    SYSCTL_RCGC2_R |= GPIO_Activate_ClkF;
    delay = SYSCTL_RCGC2_R; // Delay to stabilize clock

    // Unlock Port F and enable commit for PF0-PF4
    GPIO_PORTF_LOCK_R = GPIOF_UNLOCK;
    gpioPortF->CR = GPIOF_CR_ENABLE;

    // Loop through the predefined configuration
    for ( pin = 0; pin < sizeof(portFConfigs) / sizeof(GpioF_PinConfig_t); pin++) 
	{
        const GpioF_PinConfig_t* pinconfig = &portFConfigs[pin];
        unsigned int pinMask = (1 << pinconfig->pinNum);

        // Configure direction
        if (pinconfig->direction == GPIOF_DIR_OUTPUT) {
            gpioPortF->DIR |= pinMask;
        } else {
             gpioPortF->DIR &= ~pinMask;
        }

        // Configure pull-up resistor
        if (pinconfig->pullUp == GPIOF_PUR_ENABLE) {
             gpioPortF->PUR |= pinMask;
        } else {
             gpioPortF->PUR &= ~pinMask;
        }

        // Enable or disable digital functionality
        if (pinconfig->digitalEnable == GPIOF_DEN_ENABLE) {
             gpioPortF->DEN |= pinMask;
        } else {
             gpioPortF->DEN &= ~pinMask;
        }
				if (pinconfig->EdgeSensitive == GPIOF_EDGE_SENSITIVE) {
             gpioPortF->IS &= ~pinMask;
        } else {
             gpioPortF->IS |= pinMask;
        }
				if (pinconfig->BothEdges == GPIOF_ONE_EDGE) {
					
             gpioPortF->IBE |= pinMask;
        } else {
             gpioPortF->IBE &= ~pinMask;
        }
				if (pinconfig->FallingEdge == GPIOF_FALLING_EDGE) {
             gpioPortF->IEV &= ~pinMask;
        } else {
             gpioPortF->IEV |= pinMask;
        }
				if (pinconfig->ClearFlag == GPIOF_ICR_CLEAR) {
             gpioPortF->ICR |= pinMask;
        } else {
             gpioPortF->ICR &= ~pinMask;
        }
				if (pinconfig->InterruptMask == GPIOF_IM_ENABLE) {
             gpioPortF->IM |= pinMask;
        } else {
             gpioPortF->IM &= ~pinMask;
        }	

		}
    NVIC_PRI7_R = (NVIC_PRI7_R & 0xFF00FFFF) | 0x00A00000; // Set interrupt priority
    NVIC_EN0_R = 0x40000000;                              // Enable interrupt in NVIC
    EnableInterrupts();                                   // Enable global interrupts
}

void GPIOPortF_Handler(void)
{
    if (gpioPortF->MIS & (1 << 4)) // Check if interrupt is from SW1 (PF4)
    {
        Timer2(3); // Debounce delay
        if (!(gpioPortF->DATA & (1 << 4))) // Confirm it's a falling edge
        {
            Sw1 = 1; // Set SW1 flag
            gpioPortF->DATA |= (1 << 3); // Turn on Green LED (PF3) for debug
        }
        gpioPortF->ICR |= (1 << 4); // Clear interrupt flag for PF4
    }
    if (gpioPortF->MIS & (1 << 0)) // Check if interrupt is from SW2 (PF0)
    {
        Timer2(3); // Debounce delay
        if (!(gpioPortF->DATA & (1 << 0))) // Confirm it's a falling edge
        {
            Sw2 = 1; // Set SW2 flag
            gpioPortF->DATA |= (1 << 2); // Turn on Blue LED (PF2) for debug
        }
        gpioPortF->ICR |= (1 << 0); // Clear interrupt flag for PF0
    }
}

