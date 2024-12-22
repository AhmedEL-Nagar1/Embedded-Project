#include "..\\./headers/tm4c123gh6pm.h" 
#include "..\\./headers/Timer.h"

void Timer2_delay(unsigned long period) { 
    SYSCTL_RCGCTIMER_R |= TIMER_CLOCK_TIMER2;   // 0) activate timer2 (bit 2 of SYSCTL_RCGCTIMER_R)
   
    
    TIMER2_CTL_R &= ~TIMER_ENABLE;        // 1) disable timer2A during setup (clear bit 0 of TIMER2_CTL_R)
    TIMER2_CFG_R = TIMER_MODE_32_BIT;               // 2) configure for 32-bit mode
    TIMER2_TAMR_R = TIMER_PERIODIC_MODE;              // 3) configure for periodic mode, default down-count settings
    TIMER2_TAILR_R = period*SYSTEM_CLOCK - 1;     // 4) reload value
    
    TIMER2_ICR_R = TIMER_INTERRUPT_CLEAR;          // 6) clear timer2A timeout flag (set bit 0 of TIMER2_ICR_R)
    TIMER2_CTL_R |= TIMER_ENABLE;         // 10) enable timer2A (set bit 0 of TIMER2_CTL_R)
	
     while (!(TIMER2_RIS_R & TIMER_TIMEOUT_FLAG));  // Wait for TimerA timeout flag (check bit 0 of TIMER2_RIS_R)
      TIMER2_ICR_R = TIMER_INTERRUPT_CLEAR; 

}







