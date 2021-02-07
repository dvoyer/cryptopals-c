#pragma once

#include <iostream>
#include <cstdlib>
#include <cassert>
#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <tuple>
#include <random>

#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/applink.c>

using std::string;
using std::map;
using std::vector;
using std::tuple;

//typedefs
#include "typedefs.h"
//representation conversions
#include "conversions.h"
//general utility functions
#include "utils.h"
//everything to break vigenere ciphers specifically
#include "vigenere.h"
//aes ecb/cbc encrypt/decrypt, padding, etc.
#include "aes.h"
//oracles/specific implementations for specific challenges
#include "oracles.h"

secure_string breakAppendedECB(secure_string(*cipher)(secure_string input))
{
	// expect the cipher to produce a secure_string
	secure_string out = "";
	// detect block size
	int blkSize, j = 0;
	int minSize = cipher("").size();
	while (cipher(secure_string(j, (char)83)).size() == minSize)
		j++;
	blkSize = cipher(secure_string(j, (char)83)).size() - minSize;
	// detect ECB
	try
	{
		// ECB_or_CBC returns 0 if ECB
		if (ECB_or_CBC(cipher, blkSize))
			throw(0);
	}
	catch (int e)
	{
		std::cout << "Exception in breakAppendedECB: input is not ECB" << std::endl;
		return out;
	}
	// decrypt it
	int chunks = (int)(minSize / blkSize);
	for (int j = 0; j < chunks * blkSize; j++)
	{
		int outSize = out.size();
		secure_string trial = secure_string(chunks * blkSize - out.size() - 1, (char)83);
		secure_string enc_trial = cipher(trial);
		secure_string check_vs = enc_trial.substr(0, chunks * blkSize);
		secure_string generated;
		for (int i = 0; i < 256; i++)
		{
			generated = trial + out + secure_string(1, (char)i);
			secure_string test_string = cipher(generated);
			secure_string check_this = test_string.substr(0, chunks * blkSize);
			if (!check_vs.compare(check_this))
			{
				out += (char)i;
				break;
			}
		}
	}
	//it's always decrypting one further than the message, and i can't figure out why.
	//but it's consistent, so i can account for it here.
	return out.substr(0, out.size() - 1);
}
