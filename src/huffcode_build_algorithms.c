
#include <pq.h>
#include "huffcode.h"
#include <stdlib.h>
#include <string.h>







int huffcode_cmpFrequency(const void *arg1, const void *arg2);

int huffcode_buildHuffmanTree(struct HuffCode *hcode, unsigned int *pFrequency);

int huffcode_buildCodeTable(struct HuffCode *hcode);








int huffcode_init(struct HuffCode *hcode, unsigned int *pFrequency) {
	
	printf("Starting\n");
	huffcode_buildHuffmanTree(hcode, pFrequency);
	puts("build tree ok");
	huffcode_buildCodeTable(hcode);
	puts("build table ok");
	return 0;
}


void huffcode_destroy(struct HuffCode *hcode) {
	
	struct HuffNode *pLeaf;
	unsigned char cValue;
	unsigned char *pCodeChar;
	unsigned short iLeaf;
	
	iLeaf = 0;
	while (iLeaf < hcode->iLeafCount) {
		pLeaf = hcode->pHuffmanTree + iLeaf;
		cValue = pLeaf->cValue;
		pCodeChar = *(hcode->pCodeArray + cValue);
		free((void *) pCodeChar);
		iLeaf = iLeaf + 1;
	}
	memset((void *) hcode, 0, sizeof(struct HuffCode));
	return;
}



int huffcode_cmpFrequency(const void *arg1, const void *arg2) {
	
	unsigned int *pF1, *pF2;
	pF1 = (unsigned int *) arg1;
	pF2 = (unsigned int *) arg2;
	
	if (*pF1 > *pF2)
		return 1;
	else if (*pF1 == *pF2)
		return 0;
	else
		return -1;
}


int huffcode_buildHuffmanTree(struct HuffCode *hcode, unsigned int *pFrequency) {
	
	PriorityQueue pQueue;
	unsigned int pFreqBuffer [(2 * HUFFCODE_SIZE) - 1];
	unsigned short pIndexBuffer [(2 * HUFFCODE_SIZE) - 1];
	struct HuffNode *pNode;
	struct HuffNode *pLeft, *pRight, *pParent;
	unsigned char cNodeValue;
	unsigned int iFrequency;
	unsigned int *pFreq;
	unsigned int *pFreqLeft, *pFreqRight, *pFreqParent;
	unsigned int iIndexArray, iIndexFreq;
	unsigned short *pIndexN;
	unsigned short *pIndexL, *pIndexR, *pIndexP;
	
	iIndexFreq = 0;
	iIndexArray = 0;
	pq_init(&pQueue, PQ_HEAP_MIN, huffcode_cmpFrequency, 0, 0);
	
	while (iIndexFreq < HUFFCODE_SIZE) {
		cNodeValue = (unsigned char) iIndexFreq;
		iFrequency = *(pFrequency + iIndexFreq);
		
		if (iFrequency > 0) {
			pIndexN = pIndexBuffer + iIndexArray;
			pNode = hcode->pHuffmanTree + iIndexArray;
			pFreq = pFreqBuffer + iIndexArray;
			
			*pIndexN = (unsigned short) iIndexArray;
			*pFreq = iFrequency;
			
			pNode->cValue = cNodeValue;
			pNode->iLeftIndex = pNode->iRightIndex = 0;
			pq_insert(&pQueue, (const void *) pFreq, (const void *) pIndexN);
			iIndexArray = iIndexArray + 1;
		}
		iIndexFreq = iIndexFreq + 1;
	}
	hcode->iLeafCount = (unsigned short) iIndexArray;
	// printf("Leaf Count: %d\n", iIndexArray);
	
	while (pq_size(&pQueue) > 1) {
		pq_removeMin(&pQueue, (void **) &pFreqLeft, (void **) &pIndexL);
		pq_removeMin(&pQueue, (void **) &pFreqRight, (void **) &pIndexR);
		
		pLeft = hcode->pHuffmanTree + *pIndexL;
		pRight = hcode->pHuffmanTree + *pIndexR;
		pParent = hcode->pHuffmanTree + iIndexArray;
		pIndexP = pIndexBuffer + iIndexArray;
		pFreqParent = pFreqBuffer + iIndexArray;
		
		*pIndexP = (unsigned short) iIndexArray;
		*pFreqParent = *pFreqLeft + *pFreqRight;
		pLeft->iParentIndex = *pIndexP;
		pRight->iParentIndex = *pIndexP;
		pParent->iParentIndex = 0;
		pParent->iLeftIndex = *pIndexL;
		pParent->iRightIndex = *pIndexR;
		
		pq_insert(&pQueue, (const void *) pFreqParent, (const void *) pIndexP);
		// printf("Temp Parent Freq: %d\n", *pFreqParent);
		iIndexArray = iIndexArray + 1;
	}
	
	pq_removeMin(&pQueue, (void **) &pFreqLeft, (void **) &pIndexL);
	hcode->iRootIndex = *pIndexL;
	// printf("Root index: %d\n", *pIndexL);
	// printf("Root Key value: %d\n", *pFreqLeft);
	pq_destroy(&pQueue);
	return 0;
}


int huffcode_buildCodeTable(struct HuffCode *hcode) {
	
	unsigned char bitBuffer[1024];
	unsigned int bitCount, indexF, indexB;
	unsigned char *pCodeChar;
	unsigned short iLeaf;
	struct HuffNode *pRoot, *pLeaf, *pNode;
	unsigned short iNode;
	struct HuffNode *pParent;
	unsigned char cValue;
	
	pCodeChar = 0;
	iLeaf = 0;
	pRoot = hcode->pHuffmanTree + hcode->iRootIndex;
	memset((void *) hcode->pCodeArray, 0, HUFFCODE_SIZE * sizeof(unsigned char *));
	
	while (iLeaf < hcode->iLeafCount) {
		pLeaf = hcode->pHuffmanTree + iLeaf;
		cValue = pLeaf->cValue;
		pNode = pLeaf;
		iNode = iLeaf;
		
		bitCount = 0;
		while (pNode != pRoot) {
			pParent = hcode->pHuffmanTree + pNode->iParentIndex;
			if (pParent->iLeftIndex == iNode) {
				*(bitBuffer + bitCount) = '0';
			}
			else if (pParent->iRightIndex == iNode) {
				*(bitBuffer + bitCount) = '1';
			}
			iNode = pNode->iParentIndex;
			pNode = pParent;
			bitCount = bitCount + 1;
		}
		
		pCodeChar = (unsigned char *) malloc(bitCount + 1);
		*(pCodeChar + bitCount) = '\0';
		indexB = 0;
		indexF = bitCount;
		while (indexF > 0) {
			*(pCodeChar + indexB) = *(bitBuffer + indexF - 1);
			indexF--;
			indexB++;
		}
		*(hcode->pCodeArray + cValue) = pCodeChar;
		
		// if (cValue >= 'a' && cValue <= 'z' || cValue == ' ') {
//			printf("Table - [%c] : [%s]\n", (char) cValue, (char *) pCodeChar);
		// }
		iLeaf = iLeaf + 1;
	}
	return 0;
}


