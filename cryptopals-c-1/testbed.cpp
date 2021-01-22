#include "cryptopals.h"

void testb64Conversion();
void testHexConversion();
void testExample1();
void testExample2();
void testExample3();
void testExample4();
void testExample5();
void testHammingDistance();
void testBreakRepeatingKeyXOR();
void testExample6();

int main(int argc, char** argv)
{
	///*
	testb64Conversion();
	testHexConversion();
	testExample1();
	testExample2();
	testExample3();
	testExample4(); //takes a long time
	testExample5();
	testHammingDistance();
	testBreakRepeatingKeyXOR();
	testExample6();
	//*/
	return 0;
}

void testExample1()
{
	string testData = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
	string validate = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";
	string onePadHex = "49206e65766572207175697465207265616c697a65642e2e2e20686f772062656175746966756c207468697320776f726c642069732e2e2e";
	string validateOnePad = "SSBuZXZlciBxdWl0ZSByZWFsaXplZC4uLiBob3cgYmVhdXRpZnVsIHRoaXMgd29ybGQgaXMuLi4=";
	string twoPadHex = "57656973732c20796f752064756d6261737321205374617274206d616b696e672073656e73652c20796f7520726f7474656e20626f6f6b2c206f7220796f75276c6c20626520736f72727921";
	string validateTwoPad = "V2Vpc3MsIHlvdSBkdW1iYXNzISBTdGFydCBtYWtpbmcgc2Vuc2UsIHlvdSByb3R0ZW4gYm9vaywgb3IgeW91J2xsIGJlIHNvcnJ5IQ==";
	printf((validate.compare(hex_to_b64(testData)) || validateOnePad.compare(hex_to_b64(onePadHex)) || validateTwoPad.compare(hex_to_b64(twoPadHex))) ? "EXAMPLE 1 FAILED\n" : "hex to base 64 conversion successful\n");
	printf((testData.compare(b64_to_hex(validate)) || onePadHex.compare(b64_to_hex(validateOnePad)) || twoPadHex.compare(b64_to_hex(validateTwoPad))) ? "EXAMPLE 1 INVERSE FAILED\n" : "base 64 to hex conversion successful\n");
}


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


void testExample2()
{
	string hex1 = "1c0111001f010100061a024b53535009181c";
	string hex2 = "686974207468652062756c6c277320657965";
	string validate = "746865206b696420646f6e277420706c6179";
	printf(validate.compare(hexBufferXOR(hex1, hex2)) ? "EXAMPLE 2 FAILED\n" : "fixed xor successful\n");
}

void testExample3()
{
	string testData = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
	string validate = "436f6f6b696e67204d432773206c696b65206120706f756e64206f66206261636f6e";
	//std::cout << breakSingleByteXOR(hex_to_bin(testData)) << std::endl;
	printf(hex_to_bin(validate).compare(breakSingleByteXOR(hex_to_bin(testData))) ? "EXAMPLE 3 FAILED\n" : "broken single byte xor\n");
}

void testExample4()
{
	string validate = "4e6f77207468617420746865207061727479206973206a756d70696e670a";
	std::ifstream input("data/4.txt");
	string line;
	while ( std::getline( input, line ) )
	{
		//std::cout << line << " - " << detectSingleByteXOR(line) << std::endl;
		if (detectSingleByteXOR(hex_to_bin(line)))
		{
			break;
		}
	}
	printf(hex_to_bin(validate).compare(breakSingleByteXOR(hex_to_bin(line))) ? "EXAMPLE 4 FAILED\n" : "single byte xor detected\n");
}

void testExample5()
{
	string testData = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";
	string validate = "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272a282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27282f";
	string key = "ICE";
	//std::cout << testData << std::endl;
	printf(validate.compare(bin_to_hex(repeatingKeyXOR(testData, key))) ? "EXAMPLE 5 FAILED\n" : "repeating key xor successful\n");
}

void testHammingDistance()
{
	string str1 = "this is a test";
	string str2 = "wokka wokka!!!";
	printf((hammingDistance(str1, str2) != 37) ? "HAMMING DISTANCE FAILED\n" : "hamming distance successful\n");
}

void testBreakRepeatingKeyXOR()
{
	string testData = "I feel like I'm losing hope in my body and my soul; and the sky, it looks so ominous. And as time comes to a halt, silence starts to overflow. My cries are inconspicuous.\nTell me God, are you punishing me? Is this the price I'm paying for my past mistakes? This is my redemption song. I need you more than ever right now: Can you hear me now?\n\'Cause we\'re gonna shout it loud, even if our words seem meaningless: it\'s like I\'m carrying the weight of the world. I wish that some way, somehow, that I could save every one of us, but the truth is that I\'m only one girl...\nMaybe if I keep believing my dreams will come to life... Come to life...\nAfter all the laughter fades, signs of life all washed away, I can still, still feel a gentle breeze. No matter how hard I pray, signs of warning still remain and life has become my enemy.\nTell me God, are you punishing me? Is this the price I'm paying for my past mistakes? This is my redemption song. I need you more than ever right now: Can you hear me now?\n\'Cause we\'re gonna shout it loud, even if our words seem meaningless: it\'s like I\'m carrying the weight of the world. I wish that some way, somehow, that I could save every one of us, but the truth is that I\'m only one girl...\nMaybe if I keep believing my dreams will come to life... Come to life...\n\'Cause we\'re gonna shout it loud, even if our words seem meaningless: it\'s like I\'m carrying the weight of the world. I wish that some way, somehow, that I could save every one of us, but the truth is that I\'m only one girl...\nStill, we\'re gonna shout it loud, even if our words seem meaningless: it\'s like I\'m carrying the weight of the world. I hope that someday, somehow, that I can save every one of us, but the truth is that I\'m only one girl...\nMaybe if I keep believing my dreams will come to life... Come to life...";
	string key = "DO YOU THINK GAMES ARE SILLY LITTLE THINGS?\n> No\nYes\n";
	string cipherText = repeatingKeyXOR(testData, key);
	//std::cout << breakRepeatingKeyXOR(cipherText) << std::endl;
	printf(key.compare(breakRepeatingKeyXOR(cipherText)) ? "BREAK VIGENERE FAILED\n" : "break repeating key xor successful\n");
}

void testExample6()
{
	string validate = "5465726d696e61746f7220583a204272696e6720746865206e6f697365";
	std::ifstream input("data/6.txt");
	string line;
	string data;
	while (std::getline(input, line))
	{
		data += cleanb64(line);
	}
	//std::cout << repeatingKeyXOR(b64_to_bin(data), breakRepeatingKeyXOR(b64_to_bin(data))) << std::endl;
	printf(hex_to_bin(validate).compare(breakRepeatingKeyXOR(b64_to_bin(data))) ? "EXAMPLE 6 FAILED\n" : "repeating key xor broken from file\n");

}