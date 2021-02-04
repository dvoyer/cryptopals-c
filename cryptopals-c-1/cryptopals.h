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

// just used to validate
int _CH11_checkVal;

secure_string _CH11_encryptionOracle(secure_string input)
{
	byte KEY[KEY_SIZE];
	RAND_bytes(KEY, KEY_SIZE);
	secure_string padded = vec_to_sstring(_CH11_randXPend(sstring_to_vec(input), 5, 10, 5, 10));
	secure_string out;
	if (rand() % 2)
	{
		// CBC
		_CH11_checkVal = 1;
		byte IV[BLOCK_SIZE];
		RAND_bytes(IV, BLOCK_SIZE);
		out = aes_cbc_encrypt(KEY, IV, padded);
	}
	else
	{
		// ECB
		_CH11_checkVal = 0;
		out = aes_ecb_encrypt(KEY, padded);
	}
	return out;
}
