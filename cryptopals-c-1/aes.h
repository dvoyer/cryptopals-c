#pragma once
// reference implementation from openSSL

#include <memory>
#include <limits>
#include <stdexcept>

#include <random>

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/applink.c>

static const unsigned int KEY_SIZE = 16;
static const unsigned int BLOCK_SIZE = 16;

using EVP_CIPHER_CTX_free_ptr = std::unique_ptr<EVP_CIPHER_CTX, decltype(&::EVP_CIPHER_CTX_free)>;

void _aes_ecb_encrypt(const byte key[KEY_SIZE], const secure_string& ptext, secure_string& ctext, bool padding=true)
{
    EVP_CIPHER_CTX_free_ptr ctx(EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free);
    int rc = EVP_EncryptInit_ex(ctx.get(), EVP_aes_128_ecb(), NULL, key, NULL);
    if (rc != 1)
        throw std::runtime_error("EVP_EncryptInit_ex failed");

    if (!padding)
        EVP_CIPHER_CTX_set_padding(&*ctx, 0);

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

secure_string aes_ecb_encrypt(const byte key[KEY_SIZE], const secure_string& ptext)
{
    secure_string ctext;
    bool padding = true;
    if (ptext.size() == BLOCK_SIZE)
        padding = false;
    _aes_ecb_encrypt(key, ptext, ctext, padding);
    return ctext;
}

vector<byte> aes_ecb_encrypt(const byte key[KEY_SIZE], vector<byte> ptext)
{
    return sstring_to_vec(aes_ecb_encrypt(key, vec_to_sstring(ptext)));
}


void _aes_ecb_decrypt(const byte key[KEY_SIZE], const secure_string& ctext, secure_string& rtext, bool padding=true)
{
    try
    {        
        //BIO_dump_fp(stdout, (const char*)ctext.c_str(), ctext.size());
        EVP_CIPHER_CTX_free_ptr ctx(EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free);
        int rc = EVP_DecryptInit_ex(ctx.get(), EVP_aes_128_ecb(), NULL, key, NULL);
        if (rc != 1)
            throw std::runtime_error("EVP_DecryptInit_ex failed");

        if (!padding)
            EVP_CIPHER_CTX_set_padding(&*ctx, 0);
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
        std::cout << e.what() << " ctext:" << ctext.size() << ", rtext:" << rtext.size() << std::endl;
    }
}

secure_string aes_ecb_decrypt(const byte key[KEY_SIZE], const secure_string& ctext)
{
    secure_string rtext;
    secure_string ciphertext = ctext;
    bool padding = true;
    if (ctext.size() == BLOCK_SIZE)
        padding = false;
    _aes_ecb_decrypt(key, ciphertext, rtext, padding);
    return rtext;
}

vector<byte> aes_ecb_decrypt(const byte key[KEY_SIZE], vector<byte> ctext)
{
    return sstring_to_vec(aes_ecb_decrypt(key, vec_to_sstring(ctext)));
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

vector<byte> dechunkify(vector<vector<byte>> input)
{
    vector<byte> out;
    for (int i = 0; i < input.size(); i++)
        out.insert(out.end(), input[i].begin(), input[i].end());
    return out;
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
    //int inputsize = input.size();
    int fullblocks = (int)(input.size() / blockSize);
    int padSize = blockSize - (input.size() - fullblocks*blockSize);
    //std::cout << fullblocks << " " << padSize << " " << input.size() << std::endl;
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

vector<byte> aes_cbc_decrypt(const byte key[KEY_SIZE], const byte arrIV[BLOCK_SIZE], vector<byte> ctext)
{
    vector<byte> IV(arrIV, arrIV+BLOCK_SIZE);
    vector<byte> ctext_padded = padVector(ctext, BLOCK_SIZE); // make sure it's padded
    vector<vector<byte>> chunks = chunkify(ctext_padded, BLOCK_SIZE);
    vector<vector<byte>> dec_chunks;
    for (int i = 0; i < chunks.size(); i++)
    {
        vector<byte> previous = (i == 0 ? IV : chunks[i - 1]);
        vector<byte> decrypted = aes_ecb_decrypt(key, chunks[i]);
        vector<byte> deCBCed = vecXOR(previous, decrypted);
        //dumpVec(previous);
        //dumpVec(chunks[i]);
        //dumpVec(deCBCed);
        dec_chunks.push_back(deCBCed);
    }
    return dechunkify(dec_chunks);
}

secure_string aes_cbc_decrypt(const byte key[KEY_SIZE], const byte arrIV[BLOCK_SIZE], secure_string ctext)
{
    return vec_to_sstring(aes_cbc_decrypt(key, arrIV, sstring_to_vec(ctext)));
}

vector<byte> aes_cbc_encrypt(const byte key[KEY_SIZE], const byte arrIV[BLOCK_SIZE], vector<byte> ptext)
{
    vector<byte> IV(arrIV, arrIV + BLOCK_SIZE);
    vector<byte> ptext_padded = padVector(ptext, BLOCK_SIZE); // make sure it's padded
    vector<vector<byte>> chunks = chunkify(ptext_padded, BLOCK_SIZE);
    vector<vector<byte>> enc_chunks;
    for (int i = 0; i < chunks.size(); i++)
    {
        vector<byte> previous = (i == 0 ? IV : enc_chunks[i - 1]);
        vector<byte> enCBCed = vecXOR(previous, chunks[i]);
        vector<byte> encrypted = aes_ecb_encrypt(key, enCBCed);
        //dumpVec(previous);
        //dumpVec(chunks[i]);
        //dumpVec(encrypted);
        enc_chunks.push_back(encrypted);
    }
    return dechunkify(enc_chunks);
}

secure_string aes_cbc_encrypt(const byte key[KEY_SIZE], const byte arrIV[BLOCK_SIZE], secure_string ptext)
{
    return vec_to_sstring(aes_cbc_encrypt(key, arrIV, sstring_to_vec(ptext)));
}
