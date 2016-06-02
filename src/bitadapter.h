
#ifndef BYTE_ADAPTER_H
#define BYTE_ADAPTER_H



#include <stdio.h>




struct BitAdapter {
	
	FILE *fpFile;
	unsigned char byteValue;
	unsigned char bitIndex;
};



int bitadapter_init(struct BitAdapter *bdp, FILE *fpFIle);

void bitadapter_destroy(struct BitAdapter *bdp);


int bitadapter_flushWriteBuffer(struct BitAdapter *bdp);

int bitadapter_writeBit(struct BitAdapter *bdp, unsigned char bitChar);

int bitadapter_readBit(struct BitAdapter *bdp, unsigned char *bitChar);





#endif
