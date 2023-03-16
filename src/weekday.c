#include <avr/io.h>
#include <util/delay_basic.h>

const uint8_t buttonPin = 1;
const uint8_t ledPin = 0;

#include "weekday.h"

bool expectPush(uint32_t timeout);

int main(void)
{
  PIN_OUTPUT(ledPin);
  PIN_INPUT(buttonPin);

  while (1) {
    loop();
  }
}

uint8_t inputDigits[] = {0,0,0,0, 0,0, 0,0}; // yyyymmdd

void loop(void)
{
  // wait for first push
  waitForButton(true);

  for (uint8_t i = 0; i < 8; i++) {

    // indicate we are recording a digit
    flash();

    bool pushed;

    try_another_push:
    pushed = expectPush(300000);
    if (!pushed) {
      // move on to next digit
      continue;
    }

    inputDigits[i]++;
    // wait for release
    waitForButton(false);

    if (inputDigits[i] >= 9) {
      // move on to next digit
      continue;
    }
    goto try_another_push;
  }

  wait(3);

  uint16_t y =
    inputDigits[0]*1000 +
    inputDigits[1]*100 +
    inputDigits[2]*10 +
    inputDigits[3];
  uint8_t m =
    inputDigits[4]*10 +
    inputDigits[5];
  uint8_t d =
    inputDigits[6]*10 +
    inputDigits[7];

  uint8_t w = dateToWeekday(y, m, d);

  renderWeekday(w);
}

uint8_t dateToWeekday(uint16_t y, uint8_t m, uint8_t d)
{
  int c = (14 - m) / 12;
  int a = y - c;
  int b = m + (12 * c) - 2;
  int j = (d + a + (a / 4) - (a / 100) + (a / 400) + ((31 * b) / 12)) % 7;

  return j ?: 7;
}

void waitForButton(bool state)
{
  do {
    _delay_loop_2(100);
  } while ((!GET_BUTTON_VALUE()) == state);
}

bool expectPush(uint32_t timeout)
{
  for (uint8_t step = 100; timeout > 0; timeout -= step) {
    if (GET_BUTTON_VALUE()) {
      return true;
    }
    _delay_loop_2(step);
  }
  return false;
}

void wait(uint8_t delay)
{
  while (delay--)
  {
    _delay_loop_2(0);
  }
}

void flash(void)
{
    SET_LED(true);
    wait(1);
    SET_LED(false);
    wait(1);
}

void renderWeekday(uint8_t w)
{
  while (w--) {
    SET_LED(true);
    wait(2);
    SET_LED(false);
    wait(2);
  }
}
