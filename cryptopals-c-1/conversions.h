#pragma once

#include <iostream>
#include <cstdlib>
#include <string.h>

const char b64Alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
const char hexAlphabet[] = "0123456789abcdef";

char* hex_to_bin(char* hex)
{
	//check for correct number of hex digits
	try
	{
		if (strlen(hex) % 2 != 0)
			throw(0);
	}
	catch (int i)
	{
		std::cout << "Exception in hex_to_bin: invalid length input\n";
		exit(EXIT_FAILURE);
	}
	//convert hex chars into binary
	int binLength = strlen(hex) * 0.5;
	char* binInter;
	binInter = (char*)malloc(binLength * sizeof(char));
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
			std::cout << "Exception in hex_to_bin: invalid characters in input\n";
			exit(EXIT_FAILURE);
		}
	}
	binInter[binLength] = (char)0x00;
	return binInter;
}

char* b64_to_bin(char* b64)
{
	//check for correct number of b64 digits
	try
	{
		if (strlen(b64) % 4 != 0)
			throw(0);
	}
	catch (int i)
	{
		std::cout << "Exception in b64_to_bin: invalid length input\n";
		exit(EXIT_FAILURE);
	}
	//convert b64 chars into binary
	std::string b64Alpha = (std::string)b64Alphabet;
	char* binInter;
	binInter = (char*)malloc(strlen(b64) * 6 * 8 * sizeof(char));
	int k = 0;
	char fVals[4];
	//bool lastOct = false; //have we found padding
	for (int i = 0; i < strlen(b64); i += 4)
	{
		//std::cout << i << " ";
		fVals[0] = b64[i];
		fVals[1] = b64[i + 1];
		fVals[2] = b64[i + 2];
		fVals[3] = b64[i + 3];
		try
		{
			//I could make this faster with direct ascii comparisons, but it shouldn't be too big of a deal
			if (!(isalnum(fVals[0]) || (fVals[0] == '+') || (fVals[0] == '/')))
				throw(0);
			else if (!(isalnum(fVals[1]) || (fVals[1] == '+') || (fVals[1] == '/')))
				throw(0);
			else if (!(isalnum(fVals[2]) || (fVals[2] == '+') || (fVals[2] == '/') || (fVals[2] == '=' && fVals[3] == '=')))
				throw(0);
			else if (!(isalnum(fVals[3]) || (fVals[3] == '+') || (fVals[3] == '/') || (fVals[3] =='=')))
				throw(0);
			else
			{
				fVals[0] = b64Alpha.find(fVals[0]);
				fVals[1] = b64Alpha.find(fVals[1]);
				fVals[2] = b64Alpha.find(fVals[2]);
				fVals[3] = b64Alpha.find(fVals[3]);
				int upper = (fVals[0] << 2) + ((fVals[1] & 48) >> 4); // 00110000
				int middle;
				int lower;
				if (fVals[2] == 64)
				{
					middle = (fVals[1] << 4);
					lower = 0;
				}
				else
				{
					middle = ((fVals[1] & 15) << 4) + ((fVals[2] & 60) >> 2);
					if (fVals[3] == 64)
					{
						lower = ((fVals[2] & 3) << 6);
					}
					else
					{
						lower = ((fVals[2] & 3) << 6) + (fVals[3] & 63); // 00000011
					}
				}
				//std::cout << k << ": ";
				binInter[k] = (char)(upper);
				binInter[k + 1] = (char)(middle);
				binInter[k + 2] = (char)(lower);
				//std::cout << (char)upper << (char)(middle) << (char)(lower) << " " << bin_to_b64(foo) << "\n";
				k += 3;
			}
			//std::cout << binInter << "\n";
		}
		catch (int i)
		{
			std::cout << "Exception in b64_to_bin: invalid characters in input\n";
			exit(EXIT_FAILURE);
		}
	}
	binInter[k] = (char)0x00;
	//std::cout << binInter << "\n";
	return binInter;
}

char* bin_to_hex(char* bin)
{
	int binLength = strlen(bin);
	int hexLength = binLength * 2;
	char* hex;
	hex = (char*)malloc((hexLength + 1) * sizeof(char));
	int upper, lower;
	int k = 0;
	for (int i = 0; i < binLength; i++)
	{
		hex[k] = hexAlphabet[(int)((bin[i] & 240) >> 4)];
		hex[k + 1] = hexAlphabet[(int)(bin[i] & 15)];
		k += 2;
	}
	hex[k] = (char)0x00;
	return hex;
}

char* bin_to_b64(char* bin)
{
	int binLength = strlen(bin);
	int padLength = (binLength % 3 != 0) ? 3 - (binLength % 3) : 0;
	int b64Length = (int)(((binLength + 2) / 3) * 4);
	char* b64;
	b64 = (char*)malloc((b64Length + 1) * sizeof(char));
	//std::cout << binLength << " " << padLength << " " << b64Length << "\n";
	int bInter;
	int fVals[4];
	int k = 0;
	int maxLoop = (padLength != 0) ? binLength + padLength - 3 : binLength;
	for (int i = 0; i < maxLoop; i += 3)
	{
		bInter = (bin[i] << 16) + (bin[i + 1] << 8) + bin[i + 2];
		fVals[0] = (bInter & 16515072) >> 18;	// 111111000000000000000000
		fVals[1] = (bInter & 258048) >> 12;		// 000000111111000000000000
		fVals[2] = (bInter & 4032) >> 6;		// 000000000000111111000000
		fVals[3] = (bInter & 63);				// 000000000000000000111111
		//std::cout << " " << b64Alphabet[fVals[0]] << "" << b64Alphabet[fVals[1]] << "" << b64Alphabet[fVals[2]] << "" << b64Alphabet[fVals[3]] << "\n";
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
			bInter = bin[binLength - 1] << 4;
			fVals[0] = (bInter & 4032) >> 6;		// 000000000000111111000000
			fVals[1] = (bInter & 63);				// 000000000000000000111111
			fVals[2] = 64;	// =
			fVals[3] = 64;	// =
		}
		else
		{
			//two extra
			bInter = (bin[binLength - 2] << 16) + (bin[binLength - 1] << 8);
			fVals[0] = (bInter & 16515072) >> 18;	// 111111000000000000000000
			fVals[1] = (bInter & 258048) >> 12;		// 000000111111000000000000
			fVals[2] = (bInter & 4032) >> 6;		// 000000000000111111000000
			fVals[3] = 64;	// =

		}
		//std::cout << " " << b64Alphabet[fVals[0]] << "" << b64Alphabet[fVals[1]] << "" << b64Alphabet[fVals[2]] << "" << b64Alphabet[fVals[3]] << "\n";
		for (int j = 0; j < 4; j++)
		{
			b64[k] = b64Alphabet[fVals[j]];
			k++;
		}
		//std::cout << k << "\n";
	}
	b64[b64Length] = (char)0x00;
	return b64;
}

char* hex_to_b64(char* hex)
{
	return bin_to_b64(hex_to_bin(hex));
}

char* b64_to_hex(char* b64)
{
	return bin_to_hex(b64_to_bin(b64));
}