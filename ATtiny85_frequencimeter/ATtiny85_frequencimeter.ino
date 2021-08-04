volatile boolean first;
volatile boolean triggered;
volatile unsigned long overflowCount;
volatile unsigned long startTime;
volatile unsigned long finishTime;
uint32_t ZEROU = 0;

void initSetup() {
  DDRB  = B00000010;
  PORTB = B00000100;
  
  TCCR0A |= (1 << WGM01);
  TCCR0A |= (1 << WGM00);

  TCCR0A |= (1 << COM0B1);
  TCCR0A &= ~(1 << COM0B0);

  TCCR0B |= (1 << CS00);
  TCCR0B &= ~(1 << CS01);
  TCCR0B &= ~(1 << CS02);
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

  MCUCR |= (1 << ISC01);
  MCUCR &= ~(1 << ISC00);

  GIMSK |= (1 << INT0);
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

ISR(INT0_vect) {
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

void setPWM(int DC) {
  OCR0B = DC;
  prepareForInterrupts ();
}

int calcFreq(unsigned long ft, unsigned long st) {
  unsigned long elapsedTime = ft - st;
  float freq = 8000000 / float (elapsedTime);
  freq = freq * 100;
  float freqO = freq * 0.0425;
  return (int)freqO;
}

int main(void) {
  initSetup();
  prepareForInterrupts ();
  while (1) {
    if (triggered) {
      setPWM(calcFreq(finishTime, startTime));
    }else if (getZEROU() > 131000) {
      setPWM(1);
      setZEROU(0);
    }
  }
}
