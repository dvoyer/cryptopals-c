#include "cryptopals.h"

void testb64Conversion();
void testHexConversion();
void testExample1();
void testExample2();

int main(int argc, char** argv)
{
	testb64Conversion();
	testHexConversion();
	testExample1();
	
	return 0;
}

void testExample1()
{
	char testData[] = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d\0";
	char validate[] = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t\0";
	char onePadHex[] = "49206e65766572207175697465207265616c697a65642e2e2e20686f772062656175746966756c207468697320776f726c642069732e2e2e\0";
	char validateOnePad[] = "SSBuZXZlciBxdWl0ZSByZWFsaXplZC4uLiBob3cgYmVhdXRpZnVsIHRoaXMgd29ybGQgaXMuLi4=\0";
	char twoPadHex[] = "57656973732c20796f752064756d6261737321205374617274206d616b696e672073656e73652c20796f7520726f7474656e20626f6f6b2c206f7220796f75276c6c20626520736f72727921\0";
	char validateTwoPad[] = "V2Vpc3MsIHlvdSBkdW1iYXNzISBTdGFydCBtYWtpbmcgc2Vuc2UsIHlvdSByb3R0ZW4gYm9vaywgb3IgeW91J2xsIGJlIHNvcnJ5IQ==\0";
	std::cout << ((strcmp(hex_to_b64(testData), validate) || strcmp(hex_to_b64(onePadHex), validateOnePad) || strcmp(hex_to_b64(twoPadHex), validateTwoPad)) ? "EXAMPLE 1 FAILED\n" : "hex to base 64 conversion successful\n");
	std::cout << ((strcmp(b64_to_hex(validate), testData) || strcmp(b64_to_hex(validateOnePad), onePadHex) || strcmp(b64_to_hex(validateTwoPad), twoPadHex)) ? "EXAMPLE 1 INVERSE FAILED\n" : "base 64 to hex conversion successful\n");
}

void testb64Conversion()
{
	char validateNoPad[] = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t\0";
	char validateOnePad[] = "SSBuZXZlciBxdWl0ZSByZWFsaXplZC4uLiBob3cgYmVhdXRpZnVsIHRoaXMgd29ybGQgaXMuLi4=\0";
	char validateTwoPad[] = "V2Vpc3MsIHlvdSBkdW1iYXNzISBTdGFydCBtYWtpbmcgc2Vuc2UsIHlvdSByb3R0ZW4gYm9vaywgb3IgeW91J2xsIGJlIHNvcnJ5IQ==\0";
	std::cout << ((strcmp(bin_to_b64(b64_to_bin(validateNoPad)), validateNoPad) || strcmp(bin_to_b64(b64_to_bin(validateOnePad)), validateOnePad) || strcmp(bin_to_b64(b64_to_bin(validateTwoPad)), validateTwoPad)) ? "BASE 64 ENCODE/DECODE FAILED\n" : "base 64 encode/decode successful\n");
}

void testHexConversion()
{
	char testData[] = "49206e65766572207175697465207265616c697a65642e2e2e20686f772062656175746966756c207468697320776f726c642069732e2e2e\0";
	std::cout << ((strcmp(bin_to_hex(hex_to_bin(testData)), testData) ) ? "HEX ENCODE/DECODE FAILED\n" : "hex encode/decode successful\n");
}