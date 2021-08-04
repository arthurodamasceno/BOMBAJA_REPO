#ifndef UserTypes_h
#define UserTypes_h

#include "Arduino.h"

#define FILE_BASE_NAME "LOG"

const uint8_t colunas = 11;

struct data_t {
  unsigned long time;
  uint32_t dados[colunas];
};

void acquireData(data_t* data);
void printData(Print* pr, data_t* data);
void printHeader(Print* pr);
void userSetup();
#endif  // UserTypes_h
