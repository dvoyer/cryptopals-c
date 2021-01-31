#pragma once
// reference implementation from openSSL

#include <memory>
#include <limits>
#include <stdexcept>

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/applink.c>

static const unsigned int KEY_SIZE = 16;
static const unsigned int BLOCK_SIZE = 16;

using EVP_CIPHER_CTX_free_ptr = std::unique_ptr<EVP_CIPHER_CTX, decltype(&::EVP_CIPHER_CTX_free)>;

void aes_ecb_encrypt(const byte key[KEY_SIZE], const secure_string& ptext, secure_string& ctext)
{
    EVP_CIPHER_CTX_free_ptr ctx(EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free);
    int rc = EVP_EncryptInit_ex(ctx.get(), EVP_aes_128_ecb(), NULL, key, NULL);
    if (rc != 1)
        throw std::runtime_error("EVP_EncryptInit_ex failed");

    // Recovered text expands upto BLOCK_SIZE
    ctext.resize(ptext.size() + BLOCK_SIZE);
    int out_len1 = (int)ctext.size();

    rc = EVP_EncryptUpdate(ctx.get(), (byte*)&ctext[0], &out_len1, (const byte*)&ptext[0], (int)ptext.size());
    if (rc != 1)
        throw std::runtime_error("EVP_EncryptUpdate failed");

    int out_len2 = (int)ctext.size() - out_len1;
    rc = EVP_EncryptFinal_ex(ctx.get(), (byte*)&ctext[0] + out_len1, &out_len2);
    if (rc != 1)
        throw std::runtime_error("EVP_EncryptFinal_ex failed");

    // Set cipher text size now that we know it
    ctext.resize(out_len1 + out_len2);
}

void aes_ecb_decrypt(const byte key[KEY_SIZE], const secure_string& ctext, secure_string& rtext)
{
    try
    {        
        //BIO_dump_fp(stdout, (const char*)ctext.c_str(), ctext.size());
        EVP_CIPHER_CTX_free_ptr ctx(EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free);
        int rc = EVP_DecryptInit_ex(ctx.get(), EVP_aes_128_ecb(), NULL, key, NULL);
        if (rc != 1)
            throw std::runtime_error("EVP_DecryptInit_ex failed");

        // Recovered text contracts upto BLOCK_SIZE
        rtext.resize(ctext.size());
        int out_len1 = (int)rtext.size();

        rc = EVP_DecryptUpdate(ctx.get(), (byte*)&rtext[0], &out_len1, (const byte*)&ctext[0], (int)ctext.size());
        if (rc != 1)
            throw std::runtime_error("EVP_DecryptUpdate failed");

        //BIO_dump_fp(stdout, (const char*)rtext.c_str(), rtext.size());

        int out_len2 = (int)rtext.size() - out_len1;
        //std::cout << "out_len2: " << out_len2 << std::endl;
        rc = EVP_DecryptFinal_ex(ctx.get(), (byte*)&rtext[0] + out_len1, &out_len2);
        if (rc != 1)
            throw std::runtime_error("EVP_DecryptFinal_ex failed");
        //std::cout << "out_len2: " << out_len2 << std::endl;

        //BIO_dump_fp(stdout, (const char*)rtext.c_str(), rtext.size());

        // Set recovered text size now that we know it
        rtext.resize(out_len1 + out_len2);
    }
    catch (const std::runtime_error e)
    {
        std::cout << e.what() << " " << ctext.size() << " " << rtext.size() << std::endl;
    }
}

secure_string clipToBlockSize(secure_string inp)
{
    //sometimes this code just wants to add null terminators to the ends of decoded strings
    //here im getting rid of them
    secure_string clone = inp;
    //BIO_dump_fp(stdout, (const char*)clone.c_str(), clone.size());
    while (clone.size() % BLOCK_SIZE)
    {
        if (clone.back() == '\0')
            clone.pop_back();
        else
        {
            printf("Exception in clipToBlockSize: malformed input\n");
            exit(EXIT_FAILURE);
        }
    }
    return clone;
}

vector<vector<byte>> chunkify(vector<byte> inStream, int blkSize)
{
    try
    {
        if (inStream.size() % blkSize != 0)
            throw(0);
    }
    catch (int i)
    {
        printf("Exception in chunkify: input does not divide into block size evenly\n");
        exit(EXIT_FAILURE);
    }
    vector<vector<byte>> outStream;
    //std::cout << bin_to_hex(secureString_to_string(vec_to_sstring(inStream))) << std::endl;
    for (int i = 0; i < inStream.size(); i += blkSize)
    {
        vector<byte> chunk = vector<byte>(inStream.begin() + i, inStream.begin() + (i + blkSize));
        //std::cout << chunk.size() << " ";
        outStream.push_back(chunk);
    }
    //std::cout << "\n" << std::endl;
    return outStream;
}

bool detectECB(vector<byte> inputStream, int blkSize)
{
    vector<vector<byte>> chunks = chunkify(inputStream, blkSize);
    map<vector<byte>, int> chunkFreq = findDuplicates(chunks);
    if (chunkFreq.size() == 0)
        return false;
    else
        return true;
}

vector<byte> padVector(vector<byte> input, int blockSize)
{
    int padSize = blockSize - input.size();
    if (padSize > 0)
    {
        vector<byte> out = input;
        vector<byte> padding = string_to_vec(string(padSize, (byte)padSize));
        out.insert(out.end(), padding.begin(), padding.end());
        return out;
    }
    else
        return input;
}