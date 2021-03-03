/******************************************************************************
 * Definitions
 *****************************************************************************/
#ifdef __ICCSTM8__
  #define __IAR
#endif
#define __at(x) ( *(volatile uint8_t*) (x) )

/** Register locations found in the STM8S003F3 datasheet (DS7147)
 * register map */

#define CLK_CKDIVR  __at(0x50C6) /* Clock divider register */

#define PORTC_ODR   __at(0x500A) /* Port C data output latch register */
#define PORTC_IDR   __at(0x500B) /* Port C input pin value register */
#define PORTC_DDR   __at(0x500C) /* Port C data direction register */
#define PORTC_CR1   __at(0x500D) /* Port C control register 1 */
#define PORTC_CR2   __at(0x500E) /* Port C control register 2 */

#define TIM2_CR1    __at(0x5300) /* TIM2 control register 1 */
#define TIM2_IER    __at(0x5303) /* TIM2 interrupt enable register */
#define TIM2_SR1    __at(0x5304) /* TIM2 status register 1 */
#define TIM2_PSCR   __at(0x530E) /* TIM2 prescaler register */
#define TIM2_ARRH   __at(0x530F) /* TIM2 auto-reload register high 0xFF */
#define TIM2_ARRL   __at(0x5310) /* TIM2 auto-reload register low 0xFF */

#define LED_PIN     8   /* LED Pin 3 */

/** Interrupt vector mapping IRQ number for TIM2
 *  See the Table 11 Interrupt Mapping table in the datasheet for more info */
#define ON_TIM2_UPDATE_OVERFLOW   13

/******************************************************************************
 * Fixed width integers
 *****************************************************************************/

typedef unsigned char     uint8_t;
typedef unsigned short    uint16_t;
typedef unsigned long     uint32_t;

#ifdef __IAR
void SetupLEDPort(void);
void SetupTimer2Interrupt(const uint16_t preload_value);
__interrupt void timer2_isr (void);
#endif

/******************************************************************************
 * INTERRUPT SERVICE ROUTINE
 *****************************************************************************/
#ifdef __IAR
  #pragma vector = 15
  __interrupt void timer2_isr (void)
  {
    PORTC_ODR = (~PORTC_ODR ) & LED_PIN; /* Toggle LED Pin State */

    TIM2_SR1 &= 0;          /* Update Interrupt Flag (UIF), set to 0 */
  }
#endif
#ifdef __SDCC
  void timer2_isr (void) __interrupt (ON_TIM2_UPDATE_OVERFLOW)
  {
    PORTC_ODR = (~PORTC_ODR ) & LED_PIN; /* Toggle LED Pin State */

    TIM2_SR1 &= 0;          /* Update Interrupt Flag (UIF), set to 0 */
  }
#endif


/******************************************************************************
 * Local Functions
 *****************************************************************************/
void SetupLEDPort(void)
{
  PORTC_ODR |= LED_PIN;   /* Output Pin Level High (LED OFF) */
  PORTC_DDR |= LED_PIN;   /* Output Mode */
  PORTC_CR1 |= LED_PIN;   /* Push-Pull Output */
  PORTC_CR2 = 0;          /* Low speed output (2 MHz) */
}

void SetupTimer2Interrupt(const uint16_t preload_value)
{
  uint8_t arrh = (preload_value >> 8); /* 0xFA will be loaded in ARRH */
  uint8_t arrl = preload_value & 0xFF; /* 0xCE will be loaded in ARRL */

  TIM2_PSCR = 15;         /* Prescale set to 2^15 or 32768 */
  TIM2_ARRH = arrh;       /* Set High Auto-Reload Value */
  TIM2_ARRL = arrl;       /* Set Low Auto-Reload Value */
  TIM2_IER  = 1;          /* Enable Timer2 Interrupts  */
  TIM2_CR1  = 1;          /* Enable Counter enable bit (CEN) */

#ifdef __SDCC
  __asm__("rim");         /* Reset interrupt mask (enable interrupts) */
#endif
#ifdef __IAR
  __asm("rim");           /* Reset interrupt mask (enable interrupts) */
#endif
}

/******************************************************************************
 * Main Program
 *****************************************************************************/

void main ()
{
  CLK_CKDIVR = 0; /* Do not prescale HSIDIV or CPUDIV, device speed = 16 MHz */
  SetupLEDPort();

  /** To calculate TIM2 Period
   *  fOUT = fOSC / ( PSCR * ARR )
   *
   *  Where:
   *  - fOUT = Overflow Frequency (Hz)
   *  - fOSC = Clock Frequency (Hz)
   *  - PSCR = Timer 2 Prescale Value
   *  - ARR  = Auto-reload register
   *
   *  Solving for ARR:
   *  ARR = fOSC / (fOUT * PSCR)
   *
   *  ARR = 16MHz / (2Hz * 32768)
   *  ARR = 244
   *
   */

  SetupTimer2Interrupt(244);

  while(1)
  {
  }
}
