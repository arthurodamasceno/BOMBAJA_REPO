// User data functions.  Modify these functions for your data items.
#include "UserTypes.h"
#include "Wire.h"

static uint32_t startMicros;
uint8_t buff[26];

uint8_t FLAG = 0;
uint32_t s=0;
// Acquire a data record.
void acquireData(data_t* data) {
  data->sample = s;
  s++;
  data->time = micros();
  data->dados[0]  =  buff[0];
  data->dados[1]  =  buff[1];
  data->dados[2]  = (buff[2] << 8)  | (buff[3] & 0xff);
  data->dados[3]  = (buff[4] << 8)  | (buff[5] & 0xff);
  data->dados[4]  = (buff[6] << 8)  | (buff[7] & 0xff);
  data->dados[5]  = (buff[8] << 8)  | (buff[9] & 0xff);
  data->dados[6]  = (buff[10] << 8) | (buff[11] & 0xff);
  data->dados[7]  = (buff[12] << 8) | (buff[13] & 0xff);
  data->dados[8]  = (buff[14] << 24) | (buff[15] << 16)   | (buff[16] << 8) | (buff[17] & 0xff);
  data->dados[9]  = (buff[18] << 24) | (buff[19] << 16)   | (buff[20] << 8) | (buff[21] & 0xff);
  data->dados[10] = (buff[22] << 24) | (buff[23] << 16)   | (buff[24] << 8) | (buff[25] & 0xff);
}


// Manage data from I2C
void receiveEvent(int howMany)
{
  while (0 < Wire.available()) // loop through all but the last
  {
    for (int k = 0; k < 22; k++) {
      buff[k] = Wire.read();
    }

    if (buff[1] == 0xAA /*&& buff[11] == 0xBB && buff[21] == 0xCC*/) {
      s=0;
      FLAG = 1;
    }

    if (buff[0] == 0xDD /*&& buff[10] == 0xEE && buff[20] == 0xFF*/) {
      FLAG = 0;
    }
  }
}

// setup AVR I2C
void userSetup() {
  Wire.begin(8);
  Wire.setClock(400000);
  Wire.onReceive(receiveEvent);
}

// Print a data record.
void printData(Print* pr, data_t* data) {
  if (startMicros == 0) {
    startMicros = data->time;
  }
  pr->print(data->sample);
  pr->print(data->time - startMicros);
  for (int f = 0; f < 11; f++) {
    pr->write(',');
    pr->print(data->dados[f]);
  }
  pr->println();
}

// Print data header.
void printHeader(Print* pr) {
  startMicros = 0;
  pr->println(F("Sample Number (10 samples per second),UNIX Timestamp (Milliseconds since 1970-01-01),gear (),brake (),gas (),bat (),temp (),fuel (),vel (),rpm (),odometer (),lat (),lon ()"));
}
