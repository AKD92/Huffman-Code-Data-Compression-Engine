
#include "huffcode.h"






int huffcode_buildFrequency(unsigned int *pFrequency, unsigned char *pChar, unsigned int charLength)
{
	
	unsigned int index;
	unsigned int iFreq;
	unsigned char cValue;
	
	index = 0;
	while (index < charLength) {
		cValue = *(pChar + index);
		iFreq = *(pFrequency + cValue);
		iFreq = iFreq + 1;
		*(pFrequency + cValue) = iFreq;
		index = index + 1;
	}
	
	return 0;
}