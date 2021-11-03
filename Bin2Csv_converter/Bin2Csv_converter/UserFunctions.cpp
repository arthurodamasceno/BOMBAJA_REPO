#include "UserTypes.h"
// User data functions.  Modify these functions for your data items.

// Start time for data
static uint32_t startMicros;


// Print a data record.
void printData(FILE* pr, data_t* data) {
	if (startMicros == 0) {
		startMicros = data->time;
	}
	fprintf(pr, "%u", data->sample);
	
	fprintf(pr, ",%u", data->time - startMicros);
	for (int i = 0; i < ADC_DIM; i++) {
		unsigned adc = data->adc[i];
		fprintf(pr, ",%u.0", adc);
	}
	fprintf(pr, "\n");
}

// Print data header.
void printHeader(FILE* pr) {
	startMicros = 0;
	fprintf(pr, "Sample Number (10 samples per second),UNIX Timestamp (Milliseconds since 1970-01-01),gear (),brake (),gas (),bat (),temp (),fuel (),vel (),rpm (),odometer (),lat (),lon ()");
	fprintf(pr, "\n");
}
