#pragma once
#include <iostream>
#include <cstdlib>
#include <string.h>

const char b64Alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

char* hex_to_b64(char* hex)
{
	//check for correct number of hex digits
	try
	{
		if (strlen(hex) % 2 != 0)
			throw(0);
	}
	catch (int i)
	{
		std::cout << "Exception in hex_to_b64: invalid length input\n";
		exit(EXIT_FAILURE);
	}
	int binLength = strlen(hex) / 2;
	int padLength = (binLength % 3 != 0) ? 3 - (binLength % 3) : 0;
	int b64Length = (int)((strlen(hex) + 5) / 6) * 4;
	//convert hex chars into binary
	char* binInter;
	binInter = (char*)malloc(binLength * 8);
	int intChar;
	for (int i = 0; i < strlen(hex); i += 2)
	{
		try
		{
			if (hex[i] < 48 || hex[i] > 102)
				throw(0);
			else if (hex[i] > 57 && hex[i] < 97)
				throw(0);
			if (hex[i + 1] < 48 || hex[i + 1] > 102)
				throw(0);
			else if (hex[i + 1] > 57 && hex[i + 1] < 97)
				throw(0);
			else
			{
				int upper = (hex[i] > 57) ? hex[i] - 87 : hex[i] - 48;
				int lower = (hex[i + 1] > 57) ? hex[i + 1] - 87 : hex[i + 1] - 48;
				int val = upper * 16 + lower;
				binInter[i / 2] = (char)(val);
			}
		}
		catch (int i)
		{
			std::cout << "Exception in hex_to_b64: invalid characters in input\n";
			exit(EXIT_FAILURE);
		}
	}

	//read binary six bits at a time
	char* b64;
	b64 = (char*)malloc((b64Length) * sizeof(char));
	int bInter;
	int fVals[4];
	int k = 0;
	int maxLoop = (padLength != 0) ? binLength + padLength - 3 : binLength;
	for (int i = 0; i < maxLoop; i += 3)
	{
		bInter = (binInter[i] << 16) + (binInter[i + 1] << 8) + binInter[i + 2];
		fVals[0] = (bInter & 16515072) >> 18;	// 111111000000000000000000
		fVals[1] = (bInter & 258048) >> 12;		// 000000111111000000000000
		fVals[2] = (bInter & 4032) >> 6;		// 000000000000111111000000
		fVals[3] = (bInter & 63);				// 000000000000000000111111
		for (int j = 0; j < 4; j++)
		{
			//std::cout << fVals[j] << "\n";
			b64[k] = b64Alphabet[fVals[j]];
			k++;
		}
	}
	if (padLength != 0)
	{
		if (padLength == 2)
		{
			//one extra
			bInter = binInter[binLength - 1] << 4;
			fVals[0] = (bInter & 4032) >> 6;		// 000000000000111111000000
			fVals[1] = (bInter & 63);				// 000000000000000000111111
			fVals[2] = 64;	// =
			fVals[3] = 64;	// =
		}
		else
		{
			//two extra
			bInter = (binInter[binLength - 2] << 16) + (binInter[binLength - 1] << 8);
			fVals[0] = (bInter & 16515072) >> 18;	// 111111000000000000000000
			fVals[1] = (bInter & 258048) >> 12;		// 000000111111000000000000
			fVals[2] = (bInter & 4032) >> 6;		// 000000000000111111000000
			fVals[3] = 64;	// =

		}
		for (int j = 0; j < 4; j++)
		{
			b64[k] = b64Alphabet[fVals[j]];
			k++;
		}
	}
	b64[b64Length] = (char) 0x00;
	return b64;
}