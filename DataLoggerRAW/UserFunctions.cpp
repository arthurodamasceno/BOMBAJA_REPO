// User data functions.  Modify these functions for your data items.
#include "UserTypes.h"
#include "Wire.h"

static uint32_t startMicros;
uint8_t buff[22];

// Acquire a data record.
void acquireData(data_t* data) {
  data->time = micros();
  data->dados[0]  =  buff[0];
  data->dados[1]  =  buff[1];
  data->dados[2]  = (buff[2] << 8)  | (buff[3] & 0xff);
  data->dados[3]  = (buff[4] << 8)  | (buff[5] & 0xff);
  data->dados[4]  = (buff[6] << 8)  | (buff[7] & 0xff);
  data->dados[5]  = (buff[8] << 8)  | (buff[9] & 0xff);
  data->dados[6]  = (buff[10] << 8) | (buff[11] & 0xff);
  data->dados[7]  = (buff[12] << 8) | (buff[13] & 0xff);
  data->dados[8]  = (buff[14] << 8) | (buff[15] << 8)   | (buff[16] << 8) | (buff[17] & 0xff);
  data->dados[9]  = (buff[18] << 8) | (buff[19] & 0xff);
  data->dados[10] = (buff[20] << 8) | (buff[21] & 0xff);
}


// Manage data from I2C
void receiveEvent(int howMany)
{
  while (0 < Wire.available()) // loop through all but the last
  {
    for (int k = 0; k < 22; k++) {
      buff[k] = Wire.read();
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
  pr->println(F("micros,gear,brake,gas,bat,temp,fuel,vel,rpm,odometer,lat,lon"));
}