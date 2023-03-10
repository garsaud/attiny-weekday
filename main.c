const uint8_t buttonPin = 1;
const uint8_t ledPin = 0;

#define DDRB (*(volatine unsigned char*)0x37)
#define PORTD (*(volatine unsigned char*)0x38)

#define PIN_OUTPUT(pin) (DDRB |= (1 << pin)
#define PIN_INPUT(pin) (DDRB &= ~(1 << pin)
#define LED_ON() (PORTD |= (1 << ledPin)
#define LED_OFF() (PORTD &= ~(1 << ledPin)
#define SET_LED(value) (value ? LED_ON() : LED_OFF())
#define GET_BUTTON_VALUE() (PORTD & (1 << buttonPin))

void setup()
{
  PIN_OUTPUT(ledPin);
  PIN_INPUT(buttonPin);

  delay(1000);
}

uint8_t inputDigits[] = {0,0,0,0, 0,0, 0,0}; // yyyymmdd

void loop()
{
  // wait for first push
  waitForButtonDown();

  for (uint8_t i = 0; i < 8; i++) {

    // indicate we are recording a digit
    flash();

    try_another_push:
    bool pushed = expectPush(1000);
    if (!pushed) {
      // move on to next digit
      continue;
    }

    inputDigits[i]++;
    waitForButtonUp();

    if (inputDigits[i] >= 9) {
      // move on to next digit
      continue;
    }
    goto try_another_push;
  }

  delay(1000);

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

void waitForButtonUp()
{
  do {
    delay(10);
  } while (GET_BUTTON_VALUE());
}

void waitForButtonDown()
{
  do {
    delay(100);
  } while (!GET_BUTTON_VALUE());
}

bool expectPush(uint16_t timeout)
{
  for (uint8_t step = 10; timeout > 0; timeout -= step) {
    if (GET_BUTTON_VALUE()) {
      return true;
    }
    delay(step);
  }
  return false;
}

void flash()
{
    SET_LED(true);
    delay(10);
    SET_LED(false);
    delay(10);
}

void renderWeekday(uint8_t w)
{
  while (w--) {
    SET_LED(true);
    delay(500);
    SET_LED(false);
    delay(500);
  }
}
