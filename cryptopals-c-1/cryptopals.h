#pragma once

#include <iostream>
#include <cstdlib>
#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <tuple>

using std::string;
using std::map;
using std::vector;
using std::tuple;

//representation conversions
#include "conversions.h"

map<char, float> englishLetterFrequency = { {'E', 12.02}, {'T' , 9.10}, {'A' , 8.12}, {'O' , 7.68}, {'I' , 7.31},
											{'N' , 6.95}, {'S' , 6.28}, {'R' , 6.02}, {'H' , 5.92}, {'D' , 4.32},
											{'L' , 3.98}, {'U' , 2.88}, {'C' , 2.71}, {'M' , 2.61}, {'F' , 2.30},
											{'Y' , 2.11}, {'W' , 2.09}, {'G' , 2.03}, {'P' , 1.82}, {'B' , 1.49},
											{'V' , 1.11}, {'K' , 0.69}, {'X' , 0.17}, {'Q' , 0.11}, {'J' , 0.10},
											{'Z' , 0.07}, {'e', 12.72}, {'t' , 9.70}, {'a' , 8.72}, {'o' , 8.28}, 
											{'i' , 7.91}, {'n' , 7.55}, {'s' , 6.88}, {'r' , 6.62}, {'h' , 6.62}, 
											{'d' , 4.82}, {'l' , 4.48}, {'u' , 3.78}, {'c' , 3.21}, {'m' , 3.11}, 
											{'f' , 3.10}, {'y' , 2.61}, {'w' , 2.59}, {'g' , 2.53}, {'p' , 2.32}, 
											{'b' , 1.99}, {'v' , 1.61}, {'k' , 0.69}, {'x' , 0.10}, {'q' , 0.09}, 
											{'j' , 0.08}, {'z' , 0.07}, {' ' , 11.5}, {'.' , 1.43}, {'\'', 0.53},
											{',' , 1.00}, {'?' , 0.50}, {'!' , 0.50}, {'\n', 0.50}, {'\"', 0.25},
											{':' , 0.50}, {';' , 0.50} }; // i don't remember where this data is from

//this is getting out of hand, now there are [three] of them!
vector<char> vecXOR(vector<char> buff1, vector<char> buff2)
{
	try
	{
		if (buff1.size() != buff2.size())
			throw(0);
	}
	catch (int i)
	{
		std::cout << "Exception in vecXOR: vectors are not the same length\n";
		exit(EXIT_FAILURE);
	}
	int buffLen = buff1.size();
	vector<char> XORbuff;
	for (int i = 0; i < buffLen; i++)
	{
		XORbuff.push_back((char)((int)buff1[i] ^ (int)buff2[i]));
	}
	return XORbuff;
}

string hexBufferXOR(string buff1, string buff2)
{
	try
	{
		if (buff1.length() != buff2.length())
			throw(0);
	}
	catch (int i)
	{
		std::cout << "Exception in hexBufferXOR: buffers are not the same length\n";
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

string charBufferXOR(string buff1, string buff2)
{
	try
	{
		if (buff1.length() != buff2.length())
			throw(0);
	}
	catch (int i)
	{
		std::cout << bin_to_hex(buff1) << std::endl << bin_to_hex(buff2) << std::endl;
		std::cout << "Exception in charBufferXOR: buffers are not the same length\n";
		exit(EXIT_FAILURE);
	}
	vector<char> XORbuff = vecXOR(vector<char>(buff1.begin(), buff1.end()), vector<char>(buff2.begin(), buff2.end()));
	return string(XORbuff.begin(), XORbuff.end());
}

float scoreText(string plainText)
{
	float score = 0;
	for (int i = 0; i < plainText.length(); i++)
	{
		if (englishLetterFrequency.find(plainText[i]) == englishLetterFrequency.end())
		{
			score -= 10000;
		}
		else
		{
			score += englishLetterFrequency[plainText[i]];
		}
	}
	return score;
}

float scoreText(vector<char> plainText)
{
	float score = 0;
	for (int i = 0; i < plainText.size(); i++)
	{
		if (englishLetterFrequency.find(plainText[i]) == englishLetterFrequency.end())
		{
			score -= 10000;
		}
		else
		{
			score += englishLetterFrequency[plainText[i]];
		}
	}
	return score;
}

tuple<int, vector<char>> _breakSingleByteXOR(vector<char> cipherText)
{
	bool test = false;
	int bestGuess[] = { 0, scoreText(cipherText) };
	for (int i = 0; i < 256; i++)
	{
		vector<char> keyStream = vector<char>(cipherText.size(), i);
		vector<char> guess = vecXOR(cipherText, keyStream);
		float score = scoreText(guess);
		if (test && score > 0)
		{
			std::cout << string(guess.begin(), guess.end()) << std::endl << " " << (char)i << " - " << score << std::endl  << std::endl;
		}
		if (score > bestGuess[1])
		{
			bestGuess[0] = i;
			bestGuess[1] = score;
		}
	}
	if (test)
	{
		system("pause");
		std::cout << "\n\n\n";
	}
	return std::make_tuple(bestGuess[0], vecXOR(cipherText, vector<char>(cipherText.size(), bestGuess[0])));
}

string breakSingleByteXOR(string cipherText)
{
	//accepts bin input
	vector<char> intVec = std::get<1>(_breakSingleByteXOR(vector<char>(cipherText.begin(), cipherText.end())));
	return string(intVec.begin(), intVec.end());
}

bool detectSingleByteXOR(string cipherText)
{
	//std::cout << "singleByte" << std::endl;
	return (scoreText(breakSingleByteXOR(cipherText)) > 0);
}

string repeatKey(string key, int strLen)
{
	string rep;
	int keyLen = key.length();
	for (int i = 0; i < strLen; i++)
		rep += key[i % keyLen];
	return rep;
}

string repeatingKeyXOR(string plainText, string key)
{
	int strLen = plainText.length();
	string keyStream = repeatKey(key, strLen);
	return charBufferXOR(plainText, keyStream);
}

int _hammingCheckBits(vector<char> bits)
{
	int count = 0;
	for (int i = 0; i < bits.size(); i++)
	{
		// cf. C Programming Language 2nd Ed. (Kernighan/Ritchie)
		int val = (int)bits[i];
		int cnt;
		for (cnt = 0; val; cnt++)
		{
			val &= val - 1; //clear least significant set bit
		}
		count += cnt;
	}
	return count;
}

int hammingDistance(string str1, string str2)
{
	vector<char> checkBits = vecXOR(vector<char>(str1.begin(), str1.end()), vector<char>(str2.begin(), str2.end()));
	return _hammingCheckBits(checkBits);
}

int hammingDistance(vector<char> str1, vector<char> str2)
{
	vector<char> checkBits = vecXOR(str1, str2);
	return _hammingCheckBits(checkBits);
}

int _guessKeyLength(string cipherText)
{
	//accepts binary input
	int nBlocks;
	int keySize;
	float editDist;
	float cmp;
	vector<char> cText(cipherText.begin(), cipherText.end());
	vector<char> ch1, ch2;
	for (int i = 2; i < (cipherText.length() / 3 ); i++)
	{
		nBlocks = (cipherText.length() / i) - 2;
		cmp = 0;
		ch1 = vector<char>(cText.begin(), cText.begin() + i);
		for (int j = 0; j < nBlocks; j++)
		{
			ch2 = vector<char>(cText.begin() + i * (j + 1), cText.begin() + i * (j + 2));
			cmp += hammingDistance(ch1, ch2) / (float)i;
		}
		cmp /= (float)(nBlocks);
		//slight penalty for longer keysizes, should remove getting n*keySize as a result here
		cmp += ((float)i / (0.9 * cipherText.length()));
		if (i == 2 || (0 < cmp && cmp < editDist) )
		{
			keySize = i;
			editDist = cmp;
		}
	}
	return keySize;
}

string breakRepeatingKeyXOR(string cipherText)
{
	// accepts binary input, returns key
	// technically single byte is repeating key with a keysize of 1
	if (detectSingleByteXOR(cipherText))
	{
		return breakSingleByteXOR(cipherText);
	}
	int keySize = _guessKeyLength(cipherText);
	vector<vector<char>> blocks, trans_blocks;
	for (int i = 0; i < (cipherText.length() / keySize) + 1; i++)
	{
		vector<char> block(cipherText.begin() + i * keySize, ( (i + 1) * keySize > cipherText.length() ? cipherText.end() : cipherText.begin() + (i + 1) * keySize));
		blocks.push_back(block);
	}
	vector<char> keyString;
	for (int i = 0; i < keySize; i++)
	{
		vector<char> trans_block;
		for (int j = 0; j < blocks.size(); j++)
		{
			if (i < blocks[j].size())
				trans_block.push_back(blocks[j][i]);
		}
		keyString.push_back(std::get<0>(_breakSingleByteXOR(trans_block)));
	}
	return string(keyString.begin(), keyString.end());
}