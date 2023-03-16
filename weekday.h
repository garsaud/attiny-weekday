#include <avr/io.h>

#ifndef DDRB
  #define DDRB (*(volatile unsigned char*)0x37)
#endif
#ifndef PORTD
  #define PORTD (*(volatile unsigned char*)0x38)
#endif

#define PIN_OUTPUT(pin) (DDRB |= (1 << pin))
#define PIN_INPUT(pin) (DDRB &= ~(1 << pin))
#define LED_ON() (PORTD |= (1 << ledPin))
#define LED_OFF() (PORTD &= ~(1 << ledPin))
#define SET_LED(value) (value ? LED_ON() : LED_OFF())
#define GET_BUTTON_VALUE() (PORTD & (1 << buttonPin))

#ifndef bool
  #define bool signed char
  #define false	0
  #define true 1
#endif

uint8_t dateToWeekday(uint16_t y, uint8_t m, uint8_t d);
