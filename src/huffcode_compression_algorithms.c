
#include "huffcode.h"
#include <string.h>
#include "bitadapter.h"
#include <stdio.h>






int huffcode_compressMemory(char *source, unsigned int srcLen,
							char *destination, unsigned int *dLen, struct HuffCode *hcode)
{
	
	unsigned char cSrc, *cCode;
	unsigned int iSrc, iDest;
	unsigned int iCodeLen;
	
	iSrc = iDest = 0;
	while (iSrc < srcLen) {
		cSrc = (unsigned char) *(source + iSrc);
		cCode = *(hcode->pCodeArray + cSrc);
		iCodeLen = strlen((char *) cCode);
		memcpy((void *) (destination + iDest), (const void *) cCode, iCodeLen);
		iDest += iCodeLen;
		iSrc++;
	}
	*dLen = iDest;
	return 0;
}


int huffcode_compressFile(FILE *fpRead, FILE *fpWrite, struct HuffCode *hcode)
{
	
	unsigned char cSrc, *cCode;
	unsigned int index;
	unsigned int iCodeLen;
	struct BitAdapter bdpWrite;
	unsigned char flushCount;
	
	flushCount = 0;
	fputc('\0', fpWrite);
	bitadapter_init(&bdpWrite, fpWrite);
	
	REPEAT:
	cSrc = (unsigned char) fgetc(fpRead);
	if (feof(fpRead) != 0)
		goto END;
	cCode = *(hcode->pCodeArray + cSrc);
	iCodeLen = strlen((char *) cCode);
	for (index = 0; index < iCodeLen; index++) {
		bitadapter_writeBit(&bdpWrite, *(cCode + index));
	}
	goto REPEAT;
	
	END:
	flushCount = (unsigned char) bitadapter_flushWriteBuffer(&bdpWrite);
	rewind(fpWrite);
	fputc(flushCount, fpWrite);
	bitadapter_destroy(&bdpWrite);
	
	return 0;
}

							
int huffcode_decompressMemory(char *source, unsigned int srcLen,
							char *destination, unsigned int *dLen, struct HuffCode *hcode)
{
	
	unsigned char cSrc, cDest;
	unsigned int iSrc, iDest;
	unsigned short iRoot, iCurrent;
	struct HuffNode *pRoot, *pCurrent;
	
	iSrc = iDest = 0;
	iRoot = hcode->iRootIndex;
	pRoot = hcode->pHuffmanTree + iRoot;
	iCurrent = iRoot;
	pCurrent = pRoot;
	
	while (iSrc < srcLen) {
		cSrc = (unsigned char) *(source + iSrc);
		switch (cSrc) {
			case '0':
			iCurrent = pCurrent->iLeftIndex;
			break;
			case '1':
			iCurrent = pCurrent->iRightIndex;
			break;
		}
		pCurrent = hcode->pHuffmanTree + iCurrent;
		if (iCurrent < hcode->iLeafCount) {
			*(destination + iDest) = (char) pCurrent->cValue;
			iDest = iDest + 1;
			iCurrent = iRoot;
			pCurrent = pRoot;
		}
		iSrc = iSrc + 1;
	}
	*dLen = iDest;
	return 0;
}
