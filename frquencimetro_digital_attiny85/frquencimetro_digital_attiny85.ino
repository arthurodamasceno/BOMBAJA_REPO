#define I2C_SLAVE_ADDRESS 0x5 // the 7-bit address (remember to change this when adapting this example)
// Get this from https://github.com/rambo/TinyWire
#include <TinyWireS.h>
// The default buffer size, Can't recall the scope of defines right now
#ifndef TWI_RX_BUFFER_SIZE
#define TWI_RX_BUFFER_SIZE ( 16 )
#endif

volatile boolean first;
volatile boolean triggered;
volatile unsigned long overflowCount;
volatile unsigned long startTime;
volatile unsigned long finishTime;
uint32_t ZEROU = 0;
uint8_t frequencia = 0;

uint8_t aux;

void initSetup() {

  DDRB  = B00000000;
  PORTB = B00000010;
  TinyWireS.begin(I2C_SLAVE_ADDRESS);
  TinyWireS.onRequest(requestEvent);

  //  TCCR0A |= (1 << WGM01);
  //  TCCR0A |= (1 << WGM00);

  //  TCCR0A |= (1 << COM0B1);
  //  TCCR0A &= ~(1 << COM0B0);

  //  TCCR0B |= (1 << CS00);
  //  TCCR0B &= ~(1 << CS01);
  //  TCCR0B &= ~(1 << CS02);
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
  TCCR1 |= (1 << CS10);
  TCCR1 &= ~(1 << CS11);
  TCCR1 &= ~(1 << CS12);
  TCCR1 &= ~(1 << CS13);

  // MCUCR |= (1 << ISC01);
  // MCUCR &= ~(1 << ISC00);

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

void setPWM(int DC) {
  OCR0B = DC;
  prepareForInterrupts ();
}

uint8_t calcFreq(unsigned long ft, unsigned long st) {
  unsigned long elapsedTime = ft - st;
  float freq = 8000000 / float (elapsedTime);
  //freq = freq * 100;
  //float freqO = freq * 0.0425;
  uint8_t freqO = (uint8_t)freq;
  prepareForInterrupts ();
  return freqO;
}

void requestEvent()
{
  aux--;
  TinyWireS.send(aux);
}

int main(void) {
  initSetup();
  prepareForInterrupts ();
  while (1) {
    TinyWireS_stop_check();
    if (triggered) {
      frequencia = calcFreq(finishTime, startTime);
    }
  }
}
