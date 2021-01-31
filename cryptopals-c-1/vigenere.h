#pragma once

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

float scoreText(vector<byte> plainText)
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

float scoreText(string plainText)
{
	return scoreText(string_to_vec(plainText));
}


tuple<int, vector<byte>> _breakSingleByteXOR(vector<byte> cipherText)
{
	int bestGuess[] = { 0, scoreText(cipherText) };
	for (int i = 0; i < 256; i++)
	{
		vector<byte> keyStream = vector<byte>(cipherText.size(), i);
		vector<byte> guess = vecXOR(cipherText, keyStream);
		float score = scoreText(guess);
		if (score > bestGuess[1])
		{
			bestGuess[0] = i;
			bestGuess[1] = score;
		}
	}
	return std::make_tuple(bestGuess[0], vecXOR(cipherText, vector<byte>(cipherText.size(), bestGuess[0])));
}

string breakSingleByteXOR(string cipherText)
{
	//accepts bin input
	vector<byte> intVec = std::get<1>(_breakSingleByteXOR(vector<byte>(cipherText.begin(), cipherText.end())));
	return string(intVec.begin(), intVec.end());
}

bool detectSingleByteXOR(string cipherText)
{
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

int _guessKeyLength(string cipherText)
{
	//accepts binary input
	int nBlocks;
	int keySize;
	float editDist;
	float cmp;
	vector<byte> cText(cipherText.begin(), cipherText.end());
	vector<byte> ch1, ch2;
	for (int i = 2; i < (cipherText.length() / 3); i++)
	{
		nBlocks = (cipherText.length() / i) - 2;
		cmp = 0;
		ch1 = vector<byte>(cText.begin(), cText.begin() + i);
		for (int j = 0; j < nBlocks; j++)
		{
			ch2 = vector<byte>(cText.begin() + i * (j + 1), cText.begin() + i * (j + 2));
			cmp += hammingDistance(ch1, ch2) / (float)i;
		}
		cmp /= (float)(nBlocks);
		//slight penalty for longer keysizes, should remove getting n*keySize as a result here
		cmp += ((float)i / (0.9 * cipherText.length()));
		if (i == 2 || (0 < cmp && cmp < editDist))
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
	vector<vector<byte>> blocks, trans_blocks;
	for (int i = 0; i < (cipherText.length() / keySize) + 1; i++)
	{
		vector<byte> block(cipherText.begin() + i * keySize, ((i + 1) * keySize > cipherText.length() ? cipherText.end() : cipherText.begin() + (i + 1) * keySize));
		blocks.push_back(block);
	}
	vector<char> keyString;
	for (int i = 0; i < keySize; i++)
	{
		vector<byte> trans_block;
		for (int j = 0; j < blocks.size(); j++)
		{
			if (i < blocks[j].size())
				trans_block.push_back(blocks[j][i]);
		}
		keyString.push_back(std::get<0>(_breakSingleByteXOR(trans_block)));
	}
	return string(keyString.begin(), keyString.end());
}
