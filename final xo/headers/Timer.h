#include "tm4c123gh6pm.h"
#define SYSTEM_CLOCK 1600000 // 16 MHz system clock frequency


// Clock activation for timers
typedef enum {
    TIMER_CLOCK_TIMER0 = (1 << 0),  // Activate clock for Timer 0
    TIMER_CLOCK_TIMER1 = (1 << 1),  // Activate clock for Timer 1
    TIMER_CLOCK_TIMER2 = (1 << 2),  // Activate clock for Timer 2
    TIMER_CLOCK_TIMER3 = (1 << 3),  // Activate clock for Timer 3
    TIMER_CLOCK_TIMER4 = (1 << 4),  // Activate clock for Timer 4
    TIMER_CLOCK_TIMER5 = (1 << 5)   // Activate clock for Timer 5
} Timer_Clock_t;

// Enable/Disable settings
typedef enum {
    TIMER_DISABLE = 0x00,      // Disable Timer
    TIMER_ENABLE = (1 << 0)    // Enable Timer
} Timer_Enable_t;

// Timer Bit Modes
typedef enum {
    TIMER_MODE_32_BIT = 0x00,  // Configure Timer for 32-bit mode
    TIMER_MODE_16_BIT = 0x04   // Configure Timer for 16-bit mode
} Timer_Bit_t;

// Timer Operating Modes
typedef enum {
    TIMER_ONE_SHOT_MODE = 0x01,  // Configure Timer for one-shot mode
    TIMER_PERIODIC_MODE = 0x02,  // Configure Timer for periodic mode
    TIMER_CAPTURE_MODE = 0x03    // Configure Timer for capture mode
} Timer_Mode_t;

// Timer Status Flags
typedef enum {
    TIMER_TIMEOUT_FLAG = (1 << 0)  // Timer timeout flag
} Timer_Status_t;

// Timer Interrupt Control
typedef enum {
    TIMER_INTERRUPT_CLEAR = (1 << 0),  // Clear Timer interrupt flag
    TIMER_INTERRUPT_DCLEAR            // Default/Unused clear operation
} Timer_Icr_t;

// Timer Mask Control
typedef enum {
    TIMER_INTERRUPT_MASK = (1 << 0),  // Timer interrupt mask flag
    TIMER_INTERRUPT_DMASK             // Default/Unused mask operation
} Timer_Mis_t;



void Timer2(unsigned long period);

