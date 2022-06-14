// Get this from https://github.com/arthurodamasceno/BOMBAJA_REPO

#include <TinyWireS.h>

// The default buffer size
#ifndef TWI_RX_BUFFER_SIZE
#define TWI_RX_BUFFER_SIZE ( 16 )
#endif

#define I2C_SLAVE_ADDRESS 0x04               // SPEED ADDRESS

#define CF 1.0413412475268145371238154743309 // Correction factor, 
// calibrate with reliable device

uint8_t speed_buf[] = {0xAA, 0x00, 0x00, 0x00}; // Buffer to send via TWI
uint8_t reg_position = 0x00;                    // Index of currente byte in buffer

/*Control variables*/
volatile boolean first = 0x0;
volatile boolean triggered = 0x0;
volatile uint32_t overflowCount = 0x00000000;
volatile uint32_t startTime = 0x00000000;
volatile uint32_t finishTime = 0x00000000;
uint32_t ZEROU = = 0x00000000;
uint16_t speed_val = 0x00;
uint8_t aux = 0x00;

void initSetup() {
  DDRB  = B00000000;                   // All GPIOB as inputs
  PORTB = B00000010;                   // PORTB1 (fisical pin 6) internal pull up
  TinyWireS.begin(I2C_SLAVE_ADDRESS);  // Start TWI with Speed Address
  TinyWireS.onRequest(requestEvent);   // Set up request handler
}

void prepareForInterrupts () {

  cli(); //disable all interrupts

  first = true;
  triggered = false;

  TCCR1 = 0;             // Reset Timer 1 control register
  TIFR |= (1 << TOV1);   // Set Timer 1 Overflow flag
  TCNT1 = 0;             // Timer 1 cointer reset

  overflowCount = 0;

  TIMSK |= (1 << TOIE1); // Enable overflow interrupt

  /*  Set timer pre-scaler to 8 */
  TCCR1 &= ~(1 << CS10);
  TCCR1 &= ~(1 << CS11);
  TCCR1 |=  (1 << CS12);
  TCCR1 &= ~(1 << CS13);

  PCMSK |= (1 << PCINT1); // enable pin change interrupt
  GIMSK |= (1 << PCIE);   // enable external interrupts

  sei(); //enable all interrupts
}

ISR(TIM1_OVF_vect) {
  overflowCount++;
  ZEROU++;
}

uint32_t getZEROU() {
  return ZEROU;
}

void setZEROU(uint32_t z) {
  ZEROU = z;
}

ISR(PCINT0_vect) {
  
  if (PINB & (1 << PB1)) {                    // detect rising edge
  
    uint32_t overflowCopy = overflowCount;
    setZEROU(0);
    
    if (triggered) {
      return;                                  // prevents time values to change in middle of calc.
    }

    if (first)
    {
      startTime = (overflowCopy << 8) | TCNT1; // Save first pulse timer value
      first = false;                           // Set first pulse flag
      return;
    }
    finishTime = (overflowCopy << 8) | TCNT1;  // Save second pulse timer value
    triggered = true;                          // Set trig pulse flag

    TIMSK = 0;                                 // Disable timer over flow interrupt

    GIMSK &= ~(1 << INT0);                     // Disable external interrupts
  }
}

uint16_t calcFreq(unsigned long ft, unsigned long st) {
  
  unsigned long elapsedTime = ft - st;             // Period calc.
  float freq = 1000000 * CF / float (elapsedTime); // Frequency calc.
  float speed_f = freq * ;                 /* Speed calc.
                                            * V = w*R
                                            * w = 2*pi*f
                                            * V
                                                                    */
  uint16_t speed_calc = (uint16_t) speed_f;
  
  prepareForInterrupts ();
  return speed_calc;
}

void requestEvent()
{

  TinyWireS.send(speed_buf[reg_position]);
  reg_position++;

  if (reg_position >= 3)
  {
    reg_position = 0;

  }
}

int main(void) {

  initSetup();
  prepareForInterrupts ();

  while (1) {

    TinyWireS_stop_check();

    if (triggered) {

      speed_val = calcFreq(finishTime, startTime);
      speed_buf = (speed_val >> 8);
      speed_buf = (speed_val);

    }
  }
}
