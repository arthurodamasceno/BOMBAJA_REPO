#ifndef UserTypes_h
#define UserTypes_h
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
// User data types.  Modify for your data items.
#define FILE_BASE_NAME "adc4pin"
const uint8_t ADC_DIM = 11;
struct data_t {
	uint32_t sample;
	uint32_t time;
	uint32_t adc[ADC_DIM];
};
void printData(FILE* pr, data_t* data);
void printHeader(FILE* pr);
#endif  // UserTypes_h
