#pragma once

//global AES key
byte GLOBAL_AES_KEY[KEY_SIZE] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', '0' };
bool randomizeKey = true;

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

secure_string _CH12_encryptionOracle(secure_string input)
{
	secure_string append = s_to_secureString(b64_to_bin("Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkgaGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBqdXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUgYnkK"));
	secure_string toEncrypt = input + append;
	return aes_ecb_encrypt(GLOBAL_AES_KEY, toEncrypt);
}
