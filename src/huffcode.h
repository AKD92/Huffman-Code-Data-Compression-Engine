
#ifndef HUFFMAN_CODE_H
#define HUFFMAN_CODE_H


#include <stdio.h>

#define HUFFCODE_SIZE			256




struct HuffNode {
	
	unsigned char cValue;
	unsigned short iParentIndex;
	unsigned short iLeftIndex;
	unsigned short iRightIndex;
};


struct HuffCode {
	
	unsigned short iLeafCount;
	unsigned short iRootIndex;
	unsigned char *pCodeArray [HUFFCODE_SIZE];
	struct HuffNode pHuffmanTree [(2 * HUFFCODE_SIZE) - 1];
};





int huffcode_init(struct HuffCode *hcode, unsigned int *pFrequency);

void huffcode_destroy(struct HuffCode *hcode);


int huffcode_compressFile(FILE *fpRead, FILE *fpWrite, struct HuffCode *hcode);

int huffcode_compressMemory(char *source, unsigned int srcLen,
							char *destination, unsigned int *dLen, struct HuffCode *hcode);
							
int huffcode_decompressMemory(char *source, unsigned int srcLen,
							char *destination, unsigned int *dLen, struct HuffCode *hcode);
							
							
int huffcode_buildFrequency(unsigned int *pFrequency, unsigned char *pChar, unsigned int charLength);





#endif
