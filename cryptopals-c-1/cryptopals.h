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

secure_string ECB_CutAndPaste(secure_string(*cipher_in)(secure_string input), secure_string targetInput, secure_string targetText, int bytesToChop)
{
	// detect block size
	int blkSize, j = 0;
	int minSize = staticGenerator(cipher_in, "").size();
	while (staticGenerator(cipher_in, secure_string(j, (char)83)).size() == minSize)
		j++;
	blkSize = staticGenerator(cipher_in, secure_string(j, (char)83)).size() - minSize;
	try
	{
		if (targetText.size() > blkSize)
			throw(0);
	}
	catch (int e)
	{
		std::cout << "Exception in ECB_CutAndPaste: this implementation can only cut-and-paste a maximum of " << blkSize << " bytes." << std::endl;
		return "";
	}
	// detect ECB
	try
	{
		// ECB_or_CBC returns 0 if ECB
		if (ECB_or_CBC(cipher_in, blkSize))
			throw(0);
	}
	catch (int e)
	{
		std::cout << "Exception in ECB_CutAndPaste: input is not ECB" << std::endl;
		return "";
	}
	// find the first changing block
	int first_block;
	auto baseChunks = chunkify(sstring_to_vec(staticGenerator(cipher_in, targetInput)), blkSize);
	auto np1Chunks = chunkify(sstring_to_vec(staticGenerator(cipher_in, targetInput + secure_string(1, (char)83))), blkSize);
	for (first_block = 0; first_block < baseChunks.size(); first_block++)
	{
		if (vec_to_sstring(baseChunks[first_block]).compare(vec_to_sstring(np1Chunks[first_block])) != 0)
			break;
	}
	//std::cout << first_block << std::endl;
	//pad the first blocks to make them static
	int padLengthFront;
	for (padLengthFront = 0; padLengthFront < 1000; padLengthFront++)
	{
		auto baseString = staticGenerator(cipher_in, targetInput + secure_string(padLengthFront, (char)83));
		auto np1String = staticGenerator(cipher_in, targetInput + secure_string(padLengthFront + 1, (char)83));
		if (baseString.substr(0, (first_block + 1) * blkSize).compare(np1String.substr(0, (first_block + 1) * blkSize)) == 0)
			break;
	}
	//std::cout << padLengthFront << std::endl;
	//generate the payload
	auto payload = padSString(targetText, blkSize);
	auto ECBed = chunkify(sstring_to_vec(staticGenerator(cipher_in, targetInput + secure_string(padLengthFront, (char)83) + payload)), blkSize);
	auto encryptedPayload = vec_to_sstring(ECBed[first_block + 1]);
	//find the length the input needs to be padded to to chop off the last bytesToChop bytes
	int padLengthBack = 0;
	auto baseString = staticGenerator(cipher_in, targetInput + secure_string(padLengthFront, (char)83) + secure_string(padLengthBack, (char)83));
	int baseSize = baseString.size();
	for (padLengthBack = 1; padLengthBack < blkSize * 2; padLengthBack++)
	{
		//auto np1String = staticGenerator(cipher_in, targetInput + secure_string(padLengthFront + 1, (char)83));
		if (baseSize < staticGenerator(cipher_in, targetInput + secure_string(padLengthFront, (char)83) + secure_string(padLengthBack, (char)83)).size())
			break;
	}
	auto finalPayload = secure_string(padLengthFront, (char)83) + secure_string(padLengthBack + bytesToChop, (char)83) + targetInput;
	//std::cout << finalPayload << std::endl;
	auto finalEncrypted = staticGenerator(cipher_in, finalPayload);
	auto finalVec = chunkify(sstring_to_vec(finalEncrypted), blkSize);
	finalVec.pop_back();
	auto finalString = vec_to_sstring(dechunkify(finalVec)) + encryptedPayload;
	return finalString;
}
