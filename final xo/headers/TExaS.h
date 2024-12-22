
enum DisplayType{
  UART0_Emulate_Nokia5110_NoScope,
  SSI0_Real_Nokia5110_Scope,
  SSI0_Real_Nokia5110_NoScope,
  NoLCD_NoScope
};

void TExaS_Init(enum DisplayType display);

void TExaS_Stop(void);
