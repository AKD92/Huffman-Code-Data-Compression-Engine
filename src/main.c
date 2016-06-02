#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffcode.h"

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) {
	
	unsigned int readBytes;
	char strBuffer[1024];
	unsigned int ifreq[256];
	struct HuffCode hcode;
	FILE *fpRead, *fpWrite;
	
	printf("%u\n", sizeof(ifreq));
	memset((void *) ifreq, 0, sizeof(ifreq));
	fpRead = fopen("hfman.exe", "rb");
	
	REPEAT:
	readBytes = fread((void *) strBuffer, 1, sizeof(strBuffer), fpRead);
	huffcode_buildFrequency(ifreq, (unsigned char*) strBuffer, readBytes);
	if (feof(fpRead) != 0)
		goto END;
	goto REPEAT;
	
	END:
	printf("Frequency Build Complete\n");
	fclose(fpRead);
	huffcode_init(&hcode, ifreq);
	
	fpRead = fopen("demo_original", "rb");
	fpWrite = fopen("demo_compressed", "wb");
	huffcode_compressFile(fpRead, fpWrite, &hcode);
	fclose(fpRead);
	fclose(fpWrite);
	huffcode_destroy(&hcode);
	
	return 0;
}


void demo_memory(void) {
		char *sample, *src, *dest, *decomp;
	unsigned int srcLen, destLen, dcmpLen;
	unsigned int ifreq[256];
	struct HuffCode hcode;
	
	sample = "aaaaaaaaaaaaaabbbbbbbbsssacccccbbbsbsbbscbscbsb";
	src = sample;
	dest = (char *) malloc(1024 * 3);
	decomp = (char *) malloc(1024 * 3);
	memset((void *) ifreq, 0, sizeof(unsigned int) * 256);
	
	huffcode_buildFrequency(ifreq, (unsigned char *) sample, strlen(sample));
	huffcode_init(&hcode, ifreq);
	huffcode_compressMemory(src, strlen(src), dest, &destLen, &hcode);
	huffcode_decompressMemory(dest, destLen, decomp, &dcmpLen, &hcode);
	*(dest + destLen) = '\0';
	*(decomp + dcmpLen) = '\0';
	
	printf("Source (%u): %s\n", strlen(src), src);
	printf("Compressed (%u): %s\n", destLen, dest);
	printf("De-Compressed (%u): %s\n", dcmpLen, decomp);
	
	return;
}
