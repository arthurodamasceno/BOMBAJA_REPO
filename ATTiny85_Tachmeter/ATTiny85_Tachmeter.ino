// Get this from https://github.com/arthurodamasceno/BOMBAJA_REPO

#include <TinyWireS.h>

// The default buffer size
#ifndef TWI_RX_BUFFER_SIZE
#define TWI_RX_BUFFER_SIZE ( 16 )
#endif

#define I2C_SLAVE_ADDRESS 0x05 // TACH ADDRESS

#define CF 1.0413412475268145371238154743309 //Correction factor, 
                                             //calibrate with reliable device

uint8_t frequency[]={0xAA,0x00,0x00};
byte reg_position;

volatile boolean first;
volatile boolean triggered;
volatile unsigned long overflowCount;
volatile unsigned long startTime;
volatile unsigned long finishTime;
uint32_t ZEROU = 0;
uint16_t frequencia = 0;

uint8_t aux;

void initSetup() {

  DDRB  = B00000000; // All GPIO B as inputs
  PORTB = B00000010;
  TinyWireS.begin(I2C_SLAVE_ADDRESS);
  TinyWireS.onRequest(requestEvent);
}



void prepareForInterrupts () {

  cli();
  first = true;
  triggered = false;

  TCCR1 = 0;
  TIFR |= (1 << TOV1);
  TCNT1 = 0;

  overflowCount = 0;

  TIMSK |= (1 << TOIE1);
  TCCR1 &= ~(1 << CS10);
  TCCR1 &= ~(1 << CS11);
  TCCR1 |= (1 << CS12);
  TCCR1 &= ~(1 << CS13);

  PCMSK |= (1 << PCINT1);
  GIMSK |= (1 << PCIE);
  sei();
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
  if (PINB & (1 << PB1)) { // detect rising edge
    unsigned long overflowCopy = overflowCount;
    setZEROU(0);
    if (triggered) {

      return;
    }

    if (first)
    {
      startTime = (overflowCopy << 8) | TCNT1;
      first = false;
      return;
    }
    finishTime = (overflowCopy << 8) | TCNT1;
    triggered = true;

    TIMSK = 0;

    GIMSK &= ~(1 << INT0);
  }
}

uint16_t calcFreq(unsigned long ft, unsigned long st) {
  unsigned long elapsedTime = ft - st;
  float freq = 1000000 * CF / float (elapsedTime);
  uint16_t freqO = (uint16_t)freq;
  prepareForInterrupts ();
  return freqO;
}

void requestEvent()
{
    TinyWireS.send(frequency[reg_position]);
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
      frequencia = calcFreq(finishTime, startTime);
      frequency[1] = (frequencia>>8);
      frequency[2] = (frequencia);
    }
  }
}
