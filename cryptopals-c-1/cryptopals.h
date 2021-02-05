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

byte GLOBAL_AES_KEY[KEY_SIZE] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', '0'};

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

secure_string _CH12_encryptionOracle(secure_string input)
{
	secure_string append = s_to_secureString(b64_to_bin("Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkgaGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBqdXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUgYnkK"));
	secure_string toEncrypt = input + append;
	return aes_ecb_encrypt(GLOBAL_AES_KEY, toEncrypt);
}

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
	//std::cout << bin_to_hex(cipher("")) << std::endl;
	//std::cout << bin_to_hex(cipher(secure_string(minSize, (char)83))) << std::endl;
	//std::cout << "\n\n\n\n\n";
	// decrypt it
	int chunks = (int)(minSize / blkSize);
	//std::cout << bin_to_hex(cipher("") + cipher("")) << std::endl;
	for (int j = 0; j < chunks * blkSize; j++)
	{
		int outSize = out.size();
		//std::cout << secure_string(minSize, (char)83) << std::endl;
		secure_string trial = secure_string(chunks * blkSize - out.size() - 1, (char)83);
		//std::cout << trial << std::endl;
		secure_string enc_trial = cipher(trial);
		secure_string check_vs = enc_trial.substr(0, chunks * blkSize);
		//std::cout << bin_to_hex(check_vs) << std::endl << std::endl;
		secure_string generated;
		for (int i = 0; i < 256; i++)
		{
			generated = trial + out + secure_string(1, (char)i);
			//std::cout << generated << std::endl;
			secure_string test_string = cipher(generated);
			secure_string check_this = test_string.substr(0, chunks * blkSize);
			//std::cout << bin_to_hex(check_vs) << "\n" << bin_to_hex(check_this) << "\n" << std::endl;
			if (!check_vs.compare(check_this))
			{
				//std::cout << i << " " << out.size() << std::endl;
				out += (char)i;
				//std::cout << bin_to_hex(cipher(out)) << std::endl;
				break;
			}
		}
	}
	//std::cout << out.substr(0, out.size() - 1) << std::endl;
	//it's always decrypting one further than the message, and i can't figure out why.
	//but it's consistent, so i can account for it here.
	return out.substr(0, out.size() - 1);
}
