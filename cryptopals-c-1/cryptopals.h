#pragma once

#include <iostream>
#include <cstdlib>
#include <string.h>
using std::string;

//representation conversions
#include "conversions.h"

string englishLetterFrequency = "ETAOINSRHDLUCMFYWGPBVKXQJZ"; //+32 for lower

string hexBufferXOR(string buff1, string buff2)
{
	try
	{
		if (buff1.length() != buff2.length())
			throw(0);
	}
	catch (int i)
	{
		std::cout << "Exception in bufferXOR: buffers are not the same length\n";
		exit(EXIT_FAILURE);
	}
	int buffLen = buff1.length();
	string XORbuff;
	for (int i = 0; i < buffLen; i += 2)
	{
		string ap = { (char)buff1[i], (char)buff1[i + 1] };
		string bp = { (char)buff2[i], (char)buff2[i + 1]};
		char a = hex_to_bin(ap)[0];
		char b = hex_to_bin(bp)[0];
		char x = (char)((int)a ^ (int)b);
		string heXOR = bin_to_hex(string(1, x));
		XORbuff += heXOR;
	}
	return XORbuff;
}
/*
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