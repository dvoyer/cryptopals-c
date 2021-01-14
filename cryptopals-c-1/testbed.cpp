#include "cryptopals.h"

void testb64Conversion();
void testHexConversion();
void testExample1();
void testExample2();
void testExample3();

int main(int argc, char** argv)
{
	testb64Conversion();
	testHexConversion();
	//testExample1();
	//testExample2();
	//testExample3();
	return 0;
}
/*
void testExample1()
{
	string testData = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
	string validate = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";
	string onePadHex = "49206e65766572207175697465207265616c697a65642e2e2e20686f772062656175746966756c207468697320776f726c642069732e2e2e";
	string validateOnePad = "SSBuZXZlciBxdWl0ZSByZWFsaXplZC4uLiBob3cgYmVhdXRpZnVsIHRoaXMgd29ybGQgaXMuLi4=";
	string twoPadHex = "57656973732c20796f752064756d6261737321205374617274206d616b696e672073656e73652c20796f7520726f7474656e20626f6f6b2c206f7220796f75276c6c20626520736f72727921";
	string validateTwoPad = "V2Vpc3MsIHlvdSBkdW1iYXNzISBTdGFydCBtYWtpbmcgc2Vuc2UsIHlvdSByb3R0ZW4gYm9vaywgb3IgeW91J2xsIGJlIHNvcnJ5IQ==";
	printf((strcmp(hex_to_b64(testData), validate) || strcmp(hex_to_b64(onePadHex), validateOnePad) || strcmp(hex_to_b64(twoPadHex), validateTwoPad)) ? "EXAMPLE 1 FAILED\n" : "hex to base 64 conversion successful\n");
	printf((strcmp(b64_to_hex(validate), testData) || strcmp(b64_to_hex(validateOnePad), onePadHex) || strcmp(b64_to_hex(validateTwoPad), twoPadHex)) ? "EXAMPLE 1 INVERSE FAILED\n" : "base 64 to hex conversion successful\n");
}
*/

void testb64Conversion()
{
	string validateNoPad = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";
	string validateOnePad = "SSBuZXZlciBxdWl0ZSByZWFsaXplZC4uLiBob3cgYmVhdXRpZnVsIHRoaXMgd29ybGQgaXMuLi4=";
	string validateTwoPad = "V2Vpc3MsIHlvdSBkdW1iYXNzISBTdGFydCBtYWtpbmcgc2Vuc2UsIHlvdSByb3R0ZW4gYm9vaywgb3IgeW91J2xsIGJlIHNvcnJ5IQ==";
	printf((validateNoPad.compare(bin_to_b64(b64_to_bin(validateNoPad))) || validateOnePad.compare(bin_to_b64(b64_to_bin(validateOnePad))) || validateTwoPad.compare(bin_to_b64(b64_to_bin(validateTwoPad)))) ? "BASE 64 ENCODE/DECODE FAILED\n" : "base 64 encode/decode successful\n");
}

void testHexConversion()
{
	string testData = "49206e65766572207175697465207265616c697a65642e2e2e20686f772062656175746966756c207468697320776f726c642069732e2e2e";
	printf((testData.compare(bin_to_hex(hex_to_bin(testData))) ) ? "HEX ENCODE/DECODE FAILED\n" : "hex encode/decode successful\n");
}

/*
void testExample2()
{
	string hex1 = "1c0111001f010100061a024b53535009181c";
	string hex2 = "686974207468652062756c6c277320657965";
	string validate = "746865206b696420646f6e277420706c6179";
	printf(strcmp(hexBufferXOR(hex1, hex2), validate) ? "EXAMPLE 2 FAILED\n" : "fixed xor successful\n");
}
*/
//void testExample3()
//{
//	char testData[] = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
//	breakSingleByteXOR(testData);
//}