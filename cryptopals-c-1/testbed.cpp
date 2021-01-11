#include "cryptopals.h"

void testExample1();

int main(int argc, char** argv)
{
	testExample1();
	return 0;
}

void testExample1()
{
	char testData[] = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d\0";
	char validate[] = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t\0";
	std::cout << (strcmp(hex_to_b64(testData), validate) ? "EXAMPLE 1 FAILED" : "Example 1 Passed") << "\n";
}
