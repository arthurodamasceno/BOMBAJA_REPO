#include "UserTypes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>

const uint16_t DATA_DIM = (512 - 4) / sizeof(data_t);
const uint16_t FILL_DIM = 512 - 4 - DATA_DIM * sizeof(data_t);

struct block_t {
	uint16_t count;
	uint16_t overrun;
	data_t data[DATA_DIM];
	uint8_t fill[FILL_DIM];
};

int main(int argc, char* argv[]) {
	FILE* binFile;
	FILE* csvFile;
	errno_t err;
	block_t block;

	int i;
	char buffer[5];
	printf("Digite o numero do log que deseja converter: ");
	(void)scanf_s("%d", &i);
	_itoa_s(i, buffer, 10);

	std::string x, w, k, y, f,g,o,e,z;
	x = "D:\\data";
	y = "D:\\data0";
	w = ".bin";
	f = ".csv";

	o = "D:\\teste";
	z = "D:\\teste0";
	e =	" - connection 0 - UART COM10.csv";

	if (i < 9) {
	k = y + buffer + w;
	}
	if (i > 9) {
	k = x + buffer + w;
	}

	const char* c = k.c_str();
	err = fopen_s(&binFile, c, "rb");
	if (err) {
		printf("open failed for %s\n", c);
		return 0;
	}
	if (i < 9) {
		g =z+ buffer+ e;
	}
	if (i > 9) {
		g = o+ buffer + e;
	}

	const char* t = g.c_str();
	err = fopen_s(&csvFile, t, "w");
	if (err) {
		printf("open failed for %s\n", t);
		return 0;
	}
	printHeader(csvFile);
	while (!feof(binFile)) {
		if (fread(&block, sizeof(block), 1, binFile) != 1) break;
		uint16_t i;
		if (block.count == 0 || block.count > DATA_DIM) {
			break;
		}
		if (block.overrun) {
			fprintf(csvFile, "OVERRUN,%d", block.overrun);
		}
		for (i = 0; i < block.count; i++) {
			printData(csvFile, &block.data[i]);
		}
	}
	fclose(binFile);
	fclose(csvFile);
	printf("FINALIZADO!!!!!");
	return 0;
}
