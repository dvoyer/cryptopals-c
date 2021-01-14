#pragma once

#include <iostream>
#include <cstdlib>
#include <string.h>
using std::string;

//representation conversions
#include "conversions.h"

string englishLetterFrequency = "ETAOINSRHDLUCMFYWGPBVKXQJZ"; //+32 for lower
/*
char* hexBufferXOR(char* buff1, char* buff2)
{
	try
	{
		//std::cout << buff1 << "|\n" << buff2 << "|\n";
		if (strlen(buff1) != strlen(buff2))
			throw(0);
	}
	catch (int i)
	{
		std::cout << "Exception in bufferXOR: buffers are not the same length\n";
		exit(EXIT_FAILURE);
	}
	int buffLen = strlen(buff1) + 1;
	char* XORbuff;
	XORbuff = (char*)malloc(buffLen * sizeof(char));
	//int byteLen = strlen(buff1) * sizeof(buff1[0]);
	for (int i = 0; i < buffLen; i += 2)
	{
		char ap[] = { buff1[i], buff1[i + 1], 0 };
		char bp[] = { buff2[i], buff2[i + 1], 0 };
		char a = hex_to_bin(ap)[0];
		char b = hex_to_bin(bp)[0];
		char x = (char)((int)a ^ (int)b);
		char* heXOR = bin_to_hex(&x);
		XORbuff[i] = heXOR[0];
		XORbuff[i+1] = heXOR[1];
	}
	XORbuff[buffLen - 1] = (char)0x00;
	//std::cout << XORbuff << "|" << "\n";
	return XORbuff;
}

char* breakSingleByteXOR(char* cipherText)
{
	int buffLen = strlen(hex_to_bin(cipherText)) + 1;
	int bestGuess[] = { 0, -1 };
	for (int i = 0; i < 256; i++)
	{
		std::string keyStream(buffLen, (char)i);
		printf(keyStream.c_str());
		printf("\n");
	}

	return cipherText;
}
*/