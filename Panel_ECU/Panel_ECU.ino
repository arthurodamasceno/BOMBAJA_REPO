#include <SPI.h>
#include <mcp2515.h>
#include <EEPROM.h>

#define FUEL_LED 9
#define DIST_PULSO 0.1529432025 //CALIBRAR DISTANCIA DE PULSO

uint8_t gear, brake;
uint16_t temp, vel, rpm, fuel, bat, gas;
uint32_t odometer;
int32_t lat, lon;

uint8_t fator = 0;
bool FLAG = false;
bool INIT = false;
bool od = false;
uint8_t odometer_b = 0;
float odometer_f = 0;

struct can_frame INIT_LOG;
struct can_frame STOP_LOG;
struct can_frame OD_LOG;

uint32_t anterior;

byte leds[17][16] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //0
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }, //1
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 }, //2
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1 }, //3
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 }, //4
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1 }, //5
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1 }, //6
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1 }, //7
  { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1 }, //8
  { 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, //9
  { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, //10
  { 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, //11
  { 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, //12
  { 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, //13
  { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, //14
  { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, //15
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } //16

};

int latchPin = 3;
int shiftPin = 4;
int dataPin = 2;

struct can_frame canMsg;
MCP2515 mcp2515(10);

unsigned long EEPROMReadlong(long address) {
  unsigned long four = EEPROM.read(address);
  unsigned long three = EEPROM.read(address + 1);
  unsigned long two = EEPROM.read(address + 2);
  unsigned long one = EEPROM.read(address + 3);

  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}

void EEPROMWritelong(int address, unsigned long value) {
  byte four = (value & 0xFF);
  byte three = ((value >> 8) & 0xFF);
  byte two = ((value >> 16) & 0xFF);
  byte one = ((value >> 24) & 0xFF);

  EEPROM.write(address, four);
  EEPROM.write(address + 1, three);
  EEPROM.write(address + 2, two);
  EEPROM.write(address + 3, one);
}

void TFT_val(const char* nome, uint32_t valor) //RPM,marcha,battery,pitch,roll
{
  Serial.print(nome);
  Serial.print(".val=");
  Serial.print(valor);
  Serial.write(0xff); //final de mensagem padrão
  Serial.write(0xff);
  Serial.write(0xff);

}

void ligaLeds(int numero) {
  digitalWrite(latchPin, LOW);
  for (byte count = 0; count < 16; count++)
  {
    digitalWrite(shiftPin, LOW);
    digitalWrite(dataPin, leds[numero][count]);
    digitalWrite(shiftPin, HIGH);

  }
  digitalWrite(latchPin, HIGH);
}

ISR(PCINT1_vect) {
  if (PINC & (1 << PINC0)) {    // A0 mudou de LOW para HIGH;

  }
  else { // A0 mudou de HIGH para LOW;
    FLAG = true;
  }
}

void setup() {

  // EEPROMWritelong(0, 0);

  INIT_LOG.can_id  = 0x64E;
  INIT_LOG.can_dlc = 1;
  INIT_LOG.data[0] = 0x55;

  STOP_LOG.can_id  = 0x64F;
  STOP_LOG.can_dlc = 1;
  STOP_LOG.data[0] = 0x66;

  OD_LOG.can_id  = 0x655;
  OD_LOG.can_dlc = 4;

  cli();
  //Equivalente a pinMode(A0, INPUT_PULLUP)
  DDRC &= ~(1 << DDC0); // Seta A0 como entrada;
  PORTC |= (1 << PORTC0); // Liga Pull-up;

  //setup PCINT8
  PCICR |= (1 << PCIE1);
  PCMSK1 |= (1 << PCINT8);
  sei();

  Serial.begin(115200);

  pinMode(latchPin, OUTPUT);
  pinMode(shiftPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(FUEL_LED, OUTPUT);

  mcp2515.reset();
  mcp2515.setBitrate(CAN_250KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();
}

void loop() {
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {

    if (canMsg.can_id == 0x651) {
      temp = (canMsg.data[0] << 8) | (canMsg.data[1] & 0xff);
      temp = temp / 4;
    }
    if (canMsg.can_id == 0x653) {
      vel = (canMsg.data[0] << 8) | (canMsg.data[1] & 0xff);
      //vel = (float)vel*0.55059552846;
    }
    if (canMsg.can_id == 0x654) {
      rpm = (canMsg.data[0] << 8) | (canMsg.data[1] & 0xff);
      //rpm=rpm*60;
    }
    if (canMsg.can_id == 0x652) {
      fuel = (canMsg.data[0] << 8) | (canMsg.data[1] & 0xff);
      //fuel = float(fuel)*(0.02442002442);
    }
    if (canMsg.can_id == 0x650) {
      bat = (canMsg.data[0] << 8) | (canMsg.data[1] & 0xff);
      //bat = (float)bat*(0.03669190448);
    }
    // if (canMsg.can_id == 0x655) {
    //   odometer = (canMsg.data[0] << 24) | (canMsg.data[1] << 16) | (canMsg.data[2] << 8) | (canMsg.data[3] & 0xff);
    //}
    if (canMsg.can_id == 0x664) {
      lat = (canMsg.data[0] << 24) | (canMsg.data[1] << 16) | (canMsg.data[2] << 8) | (canMsg.data[3] & 0xff);

    }
    if (canMsg.can_id == 0x665) {
      lon = (canMsg.data[0] << 24) | (canMsg.data[1] << 16) | (canMsg.data[2] << 8) | (canMsg.data[3] & 0xff);
    }
    if (canMsg.can_id == 0x657) {
      gear =  (canMsg.data[0] & 0xff);
      brake = (canMsg.data[1] & 0xff);
    }
    if (canMsg.can_id == 0x656) {
      gas = (canMsg.data[0] << 8) | (canMsg.data[1] & 0xff);
    }
    if (canMsg.can_id == 0x666) {
      odometer_b = canMsg.data[0];
      od = true;
    }
  }

  if (millis() - anterior > 100) {
    anterior = millis();
    //  TFT_val("gear", gear);
    //  TFT_val("break", brake );
    //  TFT_val("gas", gas );
    //  TFT_val("bat", bat);
    TFT_val("bat", bat);
    TFT_val("temp", temp);
    TFT_val("fuel", fuel);
    TFT_val("vel", vel);
    TFT_val("rpm", rpm);
    TFT_val("odometer", odometer);
    //  TFT_val("lat", lat);
    //  TFT_val("lon", lon);
  }

  ligaLeds(map(rpm, 0, 4500, 0, 16));
  if (fuel <= 15)digitalWrite(FUEL_LED, HIGH);
  if (fuel > 15)digitalWrite(FUEL_LED, LOW);

  if (FLAG && !INIT) {
    mcp2515.sendMessage(&INIT_LOG);
    INIT = true;
    FLAG = false;
  }
  if (FLAG && INIT) {
    mcp2515.sendMessage(&STOP_LOG);
    INIT = false;
    FLAG = false;
  }

  if (od) {
    od = false;
    odometer_f += ((float)odometer_b * DIST_PULSO);  //CALIBRAR DISTANCIA DE UM PULSO

    if (odometer_f >= 100) {
      

      odometer_f=0;
      odometer = EEPROMReadlong(0);
      odometer += 100;
      EEPROMWritelong(0, odometer);
      OD_LOG.data[0] = (uint8_t) (odometer >> 24) & 0xFF;
      OD_LOG.data[1] = (uint8_t) (odometer >> 16) & 0xFF;
      OD_LOG.data[2] = (uint8_t) (odometer >> 8) & 0xFF;
      OD_LOG.data[3] = (uint8_t) odometer & 0xFF;
      mcp2515.sendMessage(&OD_LOG);
    }
  }
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == 'C') {
      FLAG = true;
    }
  }
}
