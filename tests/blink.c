#include "stm8s003x3.h"

typedef unsigned char     uint8_t;
typedef unsigned short    uint16_t;
typedef unsigned long     uint32_t;

#define LED_PORT PORTC
#define LED_PIN  b3

void main ()
{
  CLK_CKDIVRbits.HSIDIV = HSIDIV_RC_2MHZ;
  /* CPU speed 2MHz / 128 = 15.625 kHz */
  CLK_CKDIVRbits.CPUDIV = CPUDIV_MASTER_128;

  LED_PORT.DDRbits.LED_PIN = DDR_OUTPUT_MODE;
  LED_PORT.CR1bits.LED_PIN = CR1_OUTPUT_PUSH_PULL;
  LED_PORT.CR2bits.LED_PIN = CR2_OUTPUT_SPEED_2MHZ;
  LED_PORT.ODRbits.LED_PIN = ODR_OUTPUT_HIGH ;

  while(1)
  {
    LED_PORT.ODRbits.LED_PIN ^= 1;

    for(uint16_t i = 0; i < 1000; i++) {

    }
  }
}
