// Get this from https://github.com/arthurodamasceno/BOMBAJA_REPO

#include <TinyWireS.h>

// The default buffer size
#ifndef TWI_RX_BUFFER_SIZE
#define TWI_RX_BUFFER_SIZE ( 16 )
#endif

/*INSERT FC AND TS HERE */
#define FC 1/30               //(Hz)
#define SAMPLE_TIME 1         //(s)

#define OMEGA FC*2*PI
#define ALFA OMEGA*SAMPLE_TIME

#define A ALFA/(2+ALFA)
#define B (ALFA-2)/(ALFA+2)

float FUEL = 0.0, FUEL1 = 0.0;
float count = 0.0, count1 = 0.0;
uint16_t FUEL_I=0;

#define I2C_SLAVE_ADDRESS 0x05 // FUEL ADDRESS

uint8_t FUEL_buf[]={0x55,0x00,0x00};        // Buffer to send via TWI
byte reg_position;                         // Index of current byte in buffer

volatile boolean triggered;
uint32_t overflowCount;

void initSetup(void) {

  DDRB  = B00000000;                   // All GPIO B as inputs
  PORTB = B00000010;                   // PORTB1 (fisical pin 6) internal pull up
  TinyWireS.begin(I2C_SLAVE_ADDRESS);  // Start TWI with Speed Address
  TinyWireS.onRequest(requestEvent);   // Set up request handler

  
}

void prepareForInterrupts (void) {
  cli();                   //disable all interrupts

  TCCR1 = 0;               // Reset Timer 1 control register
  TIFR |= (1 << TOV1);     // Set Timer 1 Overflow flag
  TCNT1 = 0;               // Timer 1 counter reset

  overflowCount = 0;

  TIMSK |= (1 << TOIE1);    // Enable overflow interrupt
  /*  Set timer pre-scaler to 8 */
  TCCR1 &= ~(1 << CS10);
  TCCR1 &= ~(1 << CS11);
  TCCR1 |= (1 << CS12);
  TCCR1 &= ~(1 << CS13);

  PCMSK |= (1 << PCINT1);   // enable pin change interrupt
  GIMSK |= (1 << PCIE);     // enable external interrupts
 
  triggered=false;
  sei(); 
}

ISR(TIM1_OVF_vect) {
  overflowCount++;
  if(overflowCount>3905){
    triggered = true;
    TIMSK = 0;
    GIMSK &= ~(1 << INT0);
  }
}


ISR(PCINT0_vect) {
  if (PINB & (1 << PB1)) { // detect rising edge
     count++;
  }
}

void requestEvent(void) {
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
      FUEL = count * A + count1 * A - FUEL1 * B;
      count1 = count;
      FUEL1 = FUEL;
      FUEL_I = (uint16_t) FUEL;
      FUEL_buf[1] = (FUEL_I>>8);
      FUEL_buf[2] = (FUEL_I);
      count     = 0;
      prepareForInterrupts();
    }
    
  }
  
}
