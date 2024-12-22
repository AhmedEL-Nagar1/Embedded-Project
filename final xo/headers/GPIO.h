typedef enum {
    GPIOF_PIN_0,
    GPIOF_PIN_1,
    GPIOF_PIN_2,
    GPIOF_PIN_3,
    GPIOF_PIN_4
} Gpio_Pin_t;

typedef enum {
    GPIO_Activate_ClkF = 0x00000020 
} Gpio_ActClkF_t;

typedef enum {
    GPIOF_DIR_INPUT,
    GPIOF_DIR_OUTPUT
} Gpio_Dir_t;

typedef enum  {
    GPIO_AMSEL_DISABLE,
    GPIO_AMSEL_ENABLE
} Gpio_Amsel_t;

// Enumeration for GPIO Alternate Function Select (AFSEL)
typedef enum {
    GPIO_AFSEL_DISABLE,
    GPIO_AFSEL_ENABLE
} Gpio_Afsel_t;

// Enumeration for GPIO Port Control (PCTL)
typedef enum Gpio_Pctl_e {
    GPIO_PCTL_GPIO,
    GPIO_PCTL_ALT_FUNC
} Gpio_Pctl_t;


typedef enum {
    GPIOF_PUR_DISABLE,
    GPIOF_PUR_ENABLE
} Gpio_Pur_t;

typedef enum {
    GPIOF_DEN_DISABLE,
    GPIOF_DEN_ENABLE
} Gpio_Den_t;


typedef enum {
    GPIOF_CR_DISABLE,
    GPIOF_CR_ENABLE = 0x1F
} Gpio_Cr_t;


typedef enum {
    GPIOF_LOCK,
    GPIOF_UNLOCK = 0x4C4F434B
} Gpio_Lock_t;

typedef enum {
    GPIOF_EDGE_SENSITIVE,   // GPIO interrupt is edge-sensitive
    GPIOF_LEVEL_SENSITIVE   // GPIO interrupt is level-sensitive
} Gpio_Is_t;

typedef enum {
    GPIOF_BOTH_EDGES,   // Interrupt triggered on both rising and falling edges
    GPIOF_ONE_EDGE      // Interrupt triggered on either rising or falling edge
} Gpio_Ibe_t;

typedef enum {
    GPIOF_FALLING_EDGE,  // Falling edge event (trigger on falling edge)
    GPIOF_RISING_EDGE    // Rising edge event (trigger on rising edge)
} Gpio_Iev_t;


typedef enum {
		GPIOF_ICR_DCLEAR,
    GPIOF_ICR_CLEAR    // Clear the interrupt flag for PF4 and PF0 (0x11 is the mask for PF4 and PF0)
} Gpio_Icr_t;

typedef enum {
    GPIOF_IM_DISABLE, 			 // Disable interrupts for PF4 and PF0
    GPIOF_IM_ENABLE   			// Enable interrupts for PF4 and PF0 (0x11 is the mask for PF4 and PF0)
} Gpio_Im_t;



typedef struct {
    Gpio_Pin_t 			  pinNum;
    Gpio_Dir_t 			  direction;
    Gpio_Pur_t 		 	  pullUp;
    Gpio_Den_t 			  digitalEnable;
		Gpio_Afsel_t      AlternateFunction;
		Gpio_Amsel_t      DigitalAnalogSelect;
    Gpio_Pctl_t       PortControl;
		Gpio_Is_t       	EdgeSensitive;
		Gpio_Ibe_t				BothEdges;
		Gpio_Iev_t        FallingEdge;    
    Gpio_Icr_t        ClearFlag;   
    Gpio_Im_t         InterruptMask;     
} GpioF_PinConfig_t;

// Functions
void PortF_Init(void);
void PortF_Handler(void);
void PortB_Init(void);

void EnableInterrupts(void);
void WaitForInterrupt(void);
void GPIOF_Handler(void);
