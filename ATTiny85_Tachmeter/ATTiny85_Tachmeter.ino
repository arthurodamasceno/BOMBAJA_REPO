// Get this from https://github.com/arthurodamasceno/BOMBAJA_REPO

#include <TinyWireS.h>

// The default buffer size
#ifndef TWI_RX_BUFFER_SIZE
#define TWI_RX_BUFFER_SIZE ( 16 )
#endif

#define I2C_SLAVE_ADDRESS 0x05 // TACH ADDRESS

#define RPM_COEF 60.0
#define CF 1.0413412475268145371238154743309 //Correction factor, 
                                             //calibrate with reliable device

uint8_t RPM_buf[]={0x55,0x00,0x00};        // Buffer to send via TWI
byte reg_position;                           // Index of current byte in buffer

volatile boolean first;
volatile boolean triggered;
volatile unsigned long overflowCount;
volatile unsigned long startTime;
volatile unsigned long finishTime;
uint32_t ZEROU = 0;
uint16_t RPM = 0;

uint8_t aux;

void initSetup() {

  DDRB  = B00000000;                   // All GPIO B as inputs
  PORTB = B00000010;                   // PORTB1 (fisical pin 6) internal pull up
  TinyWireS.begin(I2C_SLAVE_ADDRESS);  // Start TWI with Speed Address
  TinyWireS.onRequest(requestEvent);   // Set up request handler
}

void prepareForInterrupts () {
  cli();                   //disable all interrupts
  first = true;
  triggered = false;

  TCCR1 = 0;               // Reset Timer 1 control register
  TIFR |= (1 << TOV1);     // Set Timer 1 Overflow flag
  TCNT1 = 0;               // Timer 1 cointer reset

  overflowCount = 0;

  TIMSK |= (1 << TOIE1);    // Enable overflow interrupt
  /*  Set timer pre-scaler to 8 */
  TCCR1 &= ~(1 << CS10);
  TCCR1 &= ~(1 << CS11);
  TCCR1 |= (1 << CS12);
  TCCR1 &= ~(1 << CS13);

  PCMSK |= (1 << PCINT1);   // enable pin change interrupt
  GIMSK |= (1 << PCIE);     // enable external interrupts
  sei(); 
}

ISR(TIM1_OVF_vect) {
  overflowCount++;
  setZEROU(overflowCount);
}

uint32_t getZEROU() {
  return ZEROU;
}

void setZEROU(uint32_t z) {
  ZEROU = z;
}

ISR(PCINT0_vect) {
  if (PINB & (1 << PB1)) { // detect rising edge
    unsigned long overflowCopy = overflowCount;
    setZEROU(0);
    
    if (triggered) {

      return;
    }

    if (first)
    {
      startTime = (overflowCopy << 8);
      first = false;
      return;
    }
    finishTime = (overflowCopy << 8);
    triggered = true;

    TIMSK = 0;

    GIMSK &= ~(1 << INT0);
  }
}

uint16_t calcFreq(unsigned long ft, unsigned long st) {
  unsigned long elapsedTime = ft - st;
  float freq = 1000000 * CF / float (elapsedTime);
  uint16_t RPMO = (uint16_t)freq * RPM_COEF;  // RPM = frequency * 60 :)
  prepareForInterrupts ();
  return RPMO;
}

void requestEvent()
{
    TinyWireS.send(RPM_buf[reg_position]);
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
      RPM = calcFreq(finishTime, startTime);
      RPM_buf[1] = (RPM>>8);
      RPM_buf[2] = (RPM);
    }
    if (getZEROU()>=8000) {
      RPM = 0;
      RPM_buf[1] = (RPM>>8);
      RPM_buf[2] = (RPM);
    }
  }
}
