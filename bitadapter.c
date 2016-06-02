


#include <stdio.h>
#include <string.h>
#include "bitadapter.h"




#define MASK_READ			0x80
#define MASK_WRITE			0x01





int bitadapter_init(struct BitAdapter *bdp, FILE *fpFIle) {
	
	bdp->fpFile = fpFIle;
	bdp->byteValue = 0x00;
	bdp->bitIndex = 0;
	
	return 0;
}



void bitadapter_destroy(struct BitAdapter *bdp) {
	
	memset((void *) bdp, 0, sizeof(struct BitAdapter));
	return;
}



int bitadapter_writeBit(struct BitAdapter *bdp, unsigned char bitChar) {
	
	if (bdp->bitIndex == 0) {
		bdp->byteValue = 0x00;
	}
	
	bdp->byteValue = bdp->byteValue << 1;
	if (bitChar == '1') {
		bdp->byteValue = bdp->byteValue | MASK_WRITE;
	}
	bdp->bitIndex = (bdp->bitIndex + 1) % 8;
	
	if (bdp->bitIndex == 0) {
		if (bdp->fpFile != 0) {
			fputc((int) bdp->byteValue, bdp->fpFile);
		}
	}
	
	return 1;
}



int bitadapter_readBit(struct BitAdapter *bdp, unsigned char *bitChar) {
	
	unsigned char tmpVal;
	
	if (bdp->bitIndex == 0) {
		if (bdp->fpFile != 0) {
			bdp->byteValue = (unsigned char) fgetc(bdp->fpFile);
			if (feof(bdp->fpFile) != 0)
				return 0;
		}
	}
	
	tmpVal = !!(bdp->byteValue & MASK_READ);
	if (tmpVal == 1) {
		*bitChar = '1';
	}
	else {
		*bitChar = '0';
	}
	bdp->byteValue = bdp->byteValue << 1;
	bdp->bitIndex = (bdp->bitIndex + 1) % 8;
	
	return 1;
}


int bitadapter_flushWriteBuffer(struct BitAdapter *bdp) {
	
	int count;
	
	count = 0;
	while (bdp->bitIndex != 0) {
		bitadapter_writeBit(bdp, '0');
		count = count + 1;
	}
	
	return count;
}


