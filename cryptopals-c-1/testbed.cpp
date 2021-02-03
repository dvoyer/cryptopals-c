#include "cryptopals.h"
#include "testStrings.h"
#include "testSet1.h"

void testExample9();
void testCBCEncryptDecrypt();
void testExample10();
void testExample11();

int main(int argc, char** argv)
{
	///*
	testSet1();
	//*/
	testExample9();
	testCBCEncryptDecrypt();
	testExample10();
	return 0;
}

void testExample9()
{
	vector<byte> testData = string_to_vec("YELLOW SUBMARINE");
	string validate = "YELLOW SUBMARINE\x04\x04\x04\x04";
	string processed = secureString_to_string(vec_to_sstring(padVector(testData, 20)));
	std::cout << (validate.compare(processed) ? "EXAMPLE 9 FAILED\n" : "pkcs#7 padding successful\n");
}

void testCBCEncryptDecrypt()
{
	secure_string validate = "Maybe if I keep believing my dreams will come to life... Come to life...";
	byte key[] = " NieR: Automata ";
	byte IV[] =  "+it^always?ends-";
	secure_string ctext, ptext;
	ctext = aes_cbc_encrypt(key, IV, WeightOfTheWorld_s);
	ptext = aes_cbc_decrypt(key, IV, ctext);
	std::cout << (validate.compare(ptext.substr(1755, 72)) ? "OPENSSL CBC ENCRYPT/DECRYPT FAILED\n" : "cbc encrypt/decrypt successful\n");
}

void testExample10()
{
	ERR_print_errors_fp(stderr);
	string validate = "Come on, Come on \nPlay that funky music \n";
	byte key[] = "YELLOW SUBMARINE";
	byte IV[] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
	secure_string ctext;
	std::ifstream input("data/10.txt");
	string line;
	string data;
	while (std::getline(input, line))
	{
		data += cleanb64(line);
	}
	ctext = clipToBlockSize(s_to_secureString(b64_to_bin(data)));
	assert(ctext.size() % BLOCK_SIZE == 0);
	secure_string ptext = aes_cbc_decrypt(key, IV, ctext);
	string check = ptext.c_str();
	std::cout << (validate.compare(check.substr(2835, 41)) ? "EXAMPLE 10 FAILED\n" : "cbc decrypted from file\n");
}
