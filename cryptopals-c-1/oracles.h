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

secure_string _CH13_profile_for(secure_string inp)
{
	secure_string quoted_inp = "email=";
	for (int i = 0; i < inp.length(); i++)
	{
		if (inp[i] == '&')
			quoted_inp += "\\&";
		else if (inp[i] == '=')
			quoted_inp += "\\=";
		else
			quoted_inp += inp[i];
	}
	//std::cout << quoted_inp << std::endl;
	byte _uid;
	RAND_bytes(&_uid, 1);
	int uid = (int)_uid;
	secure_string metadata = s_to_secureString("&uid=" + std::to_string(uid) + "&role=user");
	//std::cout << quoted_inp + metadata << std::endl;
	return quoted_inp + metadata;
}

secure_string _CH13_profile_for(string inp)
{
	return _CH13_profile_for(s_to_secureString(inp));
}

secure_string _CH13_helper(secure_string inp)
{
	return aes_ecb_encrypt(GLOBAL_AES_KEY, _CH13_profile_for(inp));
}

bool _CH13_detectAdmin(secure_string inp)
{
	auto cookie = kvParse(inp);
	for (int i = 0; i < cookie.size(); i++)
	{
		if (std::get<0>(cookie[i]).compare("role") == 0 && std::get<1>(cookie[i]).compare("admin") == 0)
			return true;
	}
	return false;
}
