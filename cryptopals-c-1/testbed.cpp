#include "cryptopals.h"
#include "testStrings.h"
#include "testSet1.h"

void testExample9();

int main(int argc, char** argv)
{
	///*
	testSet1();
	//*/
	testExample9();
	return 0;
}

void testExample9()
{
	vector<byte> testData = string_to_vec("YELLOW SUBMARINE");
	string validate = "YELLOW SUBMARINE\x04\x04\x04\x04";
	string processed = secureString_to_string(vec_to_sstring(padVector(testData, 20)));
	std::cout << (validate.compare(processed) ? "EXAMPLE 9 FAILED\n" : "pkcs#7 padding successful\n");
}
