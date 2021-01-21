#pragma once

#include <iostream>
#include <cstdlib>
#include <string>
#include <map>
#include <fstream>

using std::string;
using std::map;

//representation conversions
#include "conversions.h"

map<char, float> englishLetterFrequency = { {'E', 12.02}, {'T' , 9.10}, {'A' , 8.12}, {'O' , 7.68}, {'I' , 7.31}, 
											{'N' , 6.95}, {'S' , 6.28}, {'R' , 6.02}, {'H' , 5.92}, {'D' , 4.32}, 
											{'L' , 3.98}, {'U' , 2.88}, {'C' , 2.71}, {'M' , 2.61}, {'F' , 2.30},
											{'Y' , 2.11}, {'W' , 2.09}, {'G' , 2.03}, {'P' , 1.82}, {'B' , 1.49},
											{'V' , 1.11}, {'K' , 0.69}, {'X' , 0.17}, {'Q' , 0.11}, {'J' , 0.10}, 
											{'Z' , 0.07}, {' ' , 1 } }; // i don't remember where this data is from

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

float scoreText(string plainText)
{
	float score = 0;
	for (int i = 0; i < plainText.length(); i++)
	{
		if (englishLetterFrequency.find(plainText[i]) == englishLetterFrequency.end())
		{
			if (englishLetterFrequency.find(plainText[i] - 32) == englishLetterFrequency.end())
			{
				score -= 24.02;
			}
			else
			{
				score += englishLetterFrequency[plainText[i] - 32];
			}
		}
		else
		{
			score += englishLetterFrequency[plainText[i]];
		}
	}
	return score;
}

string breakSingleByteXOR(string cipherText)
{
	//accepts hex input
	int buffLen = cipherText.length()/2;
	int bestGuess[] = { 0, scoreText(hex_to_bin(cipherText)) }; // this gets around trying to pass \0*n to bin_to_hex
	for (int i = 1; i < 256; i++)
	{
		string keyStream(buffLen, (char)i);
		//std::cout << buffLen << " " << i << "\n" << cipherText << "\n" << (keyStream) << std::endl;
		string guess = hexBufferXOR(cipherText, bin_to_hex(keyStream));
		float score = scoreText(hex_to_bin(guess));
		if (score > bestGuess[1])
		{
			bestGuess[0] = i;
			bestGuess[1] = score;
			//std::cout << hex_to_bin(guess) << "\t\t - " << score << "\n";
		}
	}
	string keyStream(buffLen, (char)bestGuess[0]);
	return hexBufferXOR(cipherText, bin_to_hex(keyStream));
}

bool detectSingleByteXOR(string cipherText)
{
	return (scoreText(hex_to_bin(breakSingleByteXOR(cipherText))) > 0);
}
