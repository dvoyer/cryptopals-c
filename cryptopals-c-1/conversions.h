#pragma once

string b64Alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
string hexAlphabet = "0123456789abcdef";

string cleanb64(string b64)
{
	//just to remove spaces and newlines
	//O(n) bc I'm too lazy to optimize this
	string clean;
	for (int i = 0; i < b64.length(); i++)
	{
		if (b64[i] != '\n' && b64[i] != ' ' && b64[i] != '\r')
			clean += b64[i];
	}
	if (clean.back() == '\0')
	{
		clean.pop_back();
	}
	return clean;
}

string hex_to_bin(string hex)
{
	//check for correct number of hex digits
	try
	{
		if (hex.length() % 2 != 0)
			throw(0);
	}
	catch (int i)
	{
		printf("Exception in hex_to_bin: invalid length input\n");
		exit(EXIT_FAILURE);
	}
	//convert hex chars into binary
	string binInter;
	//int binLength = strlen(hex) * 0.5;
	//char* binInter;
	//binInter = (char*)malloc(binLength * sizeof(char));
	int intChar;
	for (int i = 0; i < hex.length(); i += 2)
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
				binInter += (char)val;
				//binInter[i / 2] = (char)(val);
			}
		}
		catch (int i)
		{
			printf("Exception in hex_to_bin: invalid characters in input\n");
			exit(EXIT_FAILURE);
		}
	}
	//binInter[binLength] = (char)0x00;
	return binInter;
}

string b64_to_bin(string b64)
{
	//check for correct number of b64 digits
	try
	{
		if (b64.length() % 4 != 0)
			throw(0);
	}
	catch (int i)
	{
		printf("Exception in b64_to_bin: invalid length input\n");
		exit(EXIT_FAILURE);
	}
	//convert b64 chars into binary
	string binInter;
	char fVals[4];
	for (int i = 0; i < b64.length(); i += 4)
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
				throw(1);
			else if (!(isalnum(fVals[2]) || (fVals[2] == '+') || (fVals[2] == '/') || (fVals[2] == '=' && fVals[3] == '=')))
				throw(2);
			else if (!(isalnum(fVals[3]) || (fVals[3] == '+') || (fVals[3] == '/') || (fVals[3] =='=')))
				throw(3);
			else
			{
				fVals[0] = b64Alphabet.find(fVals[0]);
				fVals[1] = b64Alphabet.find(fVals[1]);
				fVals[2] = b64Alphabet.find(fVals[2]);
				fVals[3] = b64Alphabet.find(fVals[3]);
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
				binInter += (char)(upper);
				binInter += (char)(middle);
				binInter += (char)(lower);
			}
			//std::cout << binInter << "\n";
		}
		catch (int i)
		{
			std::cout << "Exception in b64_to_bin: invalid characters in input: " << fVals[i] << "\n";
			exit(EXIT_FAILURE);
		}
	}
	return binInter;
}

string bin_to_hex(string bin)
{
	int binLength = strlen(bin.c_str()); // SHOULD be bin.length(), but it wasn't playing nice with the nul terminators
	string hex;
	int upper, lower;
	for (int i = 0; i < binLength; i++)
	{
		hex += hexAlphabet[(int)((bin[i] & 240) >> 4)];
		hex += hexAlphabet[(int)(bin[i] & 15)];
	}
	return hex;
}

secure_string bin_to_hex(secure_string bin)
{
	int binLength = bin.length();
	secure_string hex;
	int upper, lower;
	for (int i = 0; i < binLength; i++)
	{
		hex += hexAlphabet[(int)((bin[i] & 240) >> 4)];
		hex += hexAlphabet[(int)(bin[i] & 15)];
	}
	return hex;
}

secure_string bin_to_hex(vector<byte> bin)
{
	int binLength = bin.size();
	secure_string hex;
	int upper, lower;
	for (int i = 0; i < binLength; i++)
	{
		hex += hexAlphabet[(int)((bin[i] & 240) >> 4)];
		hex += hexAlphabet[(int)(bin[i] & 15)];
	}
	return hex;
}

string bin_to_b64(string bin)
{
	int binLength = strlen(bin.c_str()); // SHOULD be bin.length(), but it wasn't playing nice with the nul terminators
	int padLength = (binLength % 3 != 0) ? 3 - (binLength % 3) : 0;
	string b64Out = "";
	int bInter;
	int maxLoop = (padLength != 0) ? binLength + padLength - 3 : binLength;
	for (int i = 0; i < maxLoop; i += 3)
	{
		bInter = (bin[i] << 16) + (bin[i + 1] << 8) + bin[i + 2];
		//printf("%i ", bInter);
		b64Out += b64Alphabet[(bInter & 16515072) >> 18];	// 111111000000000000000000
		b64Out += b64Alphabet[(bInter & 258048) >> 12];		// 000000111111000000000000
		b64Out += b64Alphabet[(bInter & 4032) >> 6];		// 000000000000111111000000
		b64Out += b64Alphabet[(bInter & 63)];				// 000000000000000000111111
	}
	if (padLength != 0)
	{
		if (padLength == 2)
		{
			//one extra
			bInter = bin[binLength - 1] << 4;
			b64Out += b64Alphabet[(bInter & 4032) >> 6];		// 000000000000111111000000
			b64Out += b64Alphabet[(bInter & 63)];				// 000000000000000000111111
			b64Out += "==";
		}
		else
		{
			//two extra
			bInter = (bin[binLength - 2] << 16) + (bin[binLength - 1] << 8);
			b64Out += b64Alphabet[(bInter & 16515072) >> 18];	// 111111000000000000000000
			b64Out += b64Alphabet[(bInter & 258048) >> 12];		// 000000111111000000000000
			b64Out += b64Alphabet[(bInter & 4032) >> 6];		// 000000000000111111000000
			b64Out += "=";
		}
		//std::cout << k << "\n";
	}
	return b64Out;
}

secure_string bin_to_b64(secure_string bin)
{
	int binLength = bin.length(); // SHOULD be bin.length(), but it wasn't playing nice with the nul terminators
	int padLength = (binLength % 3 != 0) ? 3 - (binLength % 3) : 0;
	secure_string b64Out = "";
	int bInter;
	int maxLoop = (padLength != 0) ? binLength + padLength - 3 : binLength;
	for (int i = 0; i < maxLoop; i += 3)
	{
		bInter = (bin[i] << 16) + (bin[i + 1] << 8) + bin[i + 2];
		//printf("%i ", bInter);
		b64Out += b64Alphabet[(bInter & 16515072) >> 18];	// 111111000000000000000000
		b64Out += b64Alphabet[(bInter & 258048) >> 12];		// 000000111111000000000000
		b64Out += b64Alphabet[(bInter & 4032) >> 6];		// 000000000000111111000000
		b64Out += b64Alphabet[(bInter & 63)];				// 000000000000000000111111
	}
	if (padLength != 0)
	{
		if (padLength == 2)
		{
			//one extra
			bInter = bin[binLength - 1] << 4;
			b64Out += b64Alphabet[(bInter & 4032) >> 6];		// 000000000000111111000000
			b64Out += b64Alphabet[(bInter & 63)];				// 000000000000000000111111
			b64Out += "==";
		}
		else
		{
			//two extra
			bInter = (bin[binLength - 2] << 16) + (bin[binLength - 1] << 8);
			b64Out += b64Alphabet[(bInter & 16515072) >> 18];	// 111111000000000000000000
			b64Out += b64Alphabet[(bInter & 258048) >> 12];		// 000000111111000000000000
			b64Out += b64Alphabet[(bInter & 4032) >> 6];		// 000000000000111111000000
			b64Out += "=";
		}
		//std::cout << k << "\n";
	}
	return b64Out;
}

string hex_to_b64(string hex)
{
	return bin_to_b64(hex_to_bin(hex));
}

string b64_to_hex(string b64)
{
	return bin_to_hex(b64_to_bin(b64));
}

//TODO: figure out how to make these all implicit and not have to call it constantly

secure_string s_to_secureString(string inp)
{
	secure_string out;
	for (int i = 0; i < inp.size(); i++)
		out += (inp[i]);
	return out;
}

string secureString_to_string(secure_string inp)
{
	string out;
	for (int i = 0; i < inp.size(); i++)
		out += (inp[i]);
	return out;
}

vector<byte> string_to_vec(string inp)
{
	vector<byte> out;
	for (int i = 0; i < inp.size(); i++)
		out.push_back(inp[i]);
	return out;
}

secure_string vec_to_sstring(vector<byte> inp)
{
	secure_string out;
	for (int i = 0; i < inp.size(); i++)
		out.push_back(inp[i]);
	return out;
}

vector<byte> sstring_to_vec(secure_string inp)
{
	vector<byte> out;
	for (int i = 0; i < inp.size(); i++)
		out.push_back(inp[i]);
	return out;
}

string vec_to_string(vector<byte> inp)
{
	string out;
	for (int i = 0; i < inp.size(); i++)
		out.push_back(inp[i]);
	return out;
}

vector<byte> byteArr_to_vec(byte* inp, int size)
{
	vector<byte> out;
	for (int i = 0; i < size; i++)
		out.push_back(inp[i]);
	return out;
}

secure_string byteArr_to_sstring(byte* inp, int size)
{
	return vec_to_sstring(byteArr_to_vec(inp, size));
}

