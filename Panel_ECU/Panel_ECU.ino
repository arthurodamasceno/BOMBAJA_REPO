#include <SPI.h>
#include <mcp2515.h>

uint8_t gear, brake;
uint16_t temp, vel, rpm, fuel, bat, lat, lon, gas;
uint32_t odometer;

bool FLAG = false;
bool INIT = false;

struct can_frame INIT_LOG;
struct can_frame STOP_LOG;

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

ISR(PCINT0_vect) {
  if (PINC & (1 << PINC0)) {    // A0 mudou de LOW para HIGH;

  }
  else { // A0 mudou de HIGH para LOW;
    FLAG = true;
  }
}

void setup() {
  INIT_LOG.can_id  = 0x64E;
  INIT_LOG.can_dlc = 1;
  INIT_LOG.data[0] = 0x55;

  STOP_LOG.can_id  = 0x64F;
  STOP_LOG.can_dlc = 1;
  STOP_LOG.data[0] = 0x66;

  cli();
  //Equivalente a pinMode(A0, INPUT_PULLUP)
  DDRB &= ~(1 << DDC0); // Seta A0 como entrada;
  PORTB |= (1 << PORTC0); // Liga Pull-up;

  //setup PCINT8
  PCICR |= (1 << PCIE1);
  PCMSK0 |= (1 << PCINT0);
  sei();

  Serial.begin(115200);

  pinMode(latchPin, OUTPUT);
  pinMode(shiftPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  mcp2515.reset();
  mcp2515.setBitrate(CAN_250KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();
}

void loop() {
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {

    if (canMsg.can_id == 0x651) {
      temp = (canMsg.data[0] << 8) | (canMsg.data[1] & 0xff);
    }
    if (canMsg.can_id == 0x653) {
      vel = (canMsg.data[0] << 8) | (canMsg.data[1] & 0xff);
    }
    if (canMsg.can_id == 0x654) {
      rpm = (canMsg.data[0] << 8) | (canMsg.data[1] & 0xff);
    }
    if (canMsg.can_id == 0x652) {
      fuel = (canMsg.data[0] << 8) | (canMsg.data[1] & 0xff);
    }
    if (canMsg.can_id == 0x650) {
      bat = (canMsg.data[0] << 8) | (canMsg.data[1] & 0xff);
    }
    if (canMsg.can_id == 0x655) {
      odometer = (canMsg.data[0] << 24) | (canMsg.data[1] << 16) | (canMsg.data[2] << 8) | (canMsg.data[3] & 0xff);
    }
    if (canMsg.can_id == 0x658) {
      lat = (canMsg.data[0] << 8) | (canMsg.data[1] & 0xff);
      lon = (canMsg.data[2] << 8) | (canMsg.data[3] & 0xff);
    }
    if (canMsg.can_id == 0x657) {
      gear =  (canMsg.data[0] & 0xff);
      brake = (canMsg.data[1] & 0xff);
    }
    if (canMsg.can_id == 0x656) {
      gas = (canMsg.data[0] << 8) | (canMsg.data[1] & 0xff);
    }
  }

  if (millis() - anterior > 100) {
    anterior = millis();
    TFT_val("gear", gear);
    TFT_val("break", brake );
    TFT_val("gas", gas );
    TFT_val("bat", bat);
    TFT_val("temp", temp);
    TFT_val("fuel", fuel);
    TFT_val("vel", vel);
    TFT_val("rpm", rpm);
    TFT_val("odometer", odometer);
    TFT_val("lat", lat);
    TFT_val("lon", lon);
  }

  ligaLeds(map(rpm, 0, 4095, 0, 16));

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
}
