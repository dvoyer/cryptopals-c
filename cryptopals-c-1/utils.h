#pragma once

//this is getting out of hand, now there are [three] of them!
template <typename T>
vector<T> vecXOR(vector<T> buff1, vector<T> buff2)
{
    try
    {
        if (buff1.size() != buff2.size())
            throw(0);
    }
    catch (int i)
    {
        std::cout << "Exception in vecXOR: vectors are not the same length\n";
        exit(EXIT_FAILURE);
    }
    int buffLen = buff1.size();
    vector<T> XORbuff;
    for (int i = 0; i < buffLen; i++)
    {
        XORbuff.push_back((char)((int)buff1[i] ^ (int)buff2[i]));
    }
    return XORbuff;
}

string hexBufferXOR(string buff1, string buff2)
{
    try
    {
        if (buff1.length() != buff2.length())
            throw(0);
    }
    catch (int i)
    {
        std::cout << "Exception in hexBufferXOR: buffers are not the same length\n";
        exit(EXIT_FAILURE);
    }
    return bin_to_hex(vec_to_string(vecXOR(string_to_vec(hex_to_bin(buff1)), string_to_vec(hex_to_bin(buff2)))));
}

string charBufferXOR(string buff1, string buff2)
{
    try
    {
        if (buff1.length() != buff2.length())
            throw(0);
    }
    catch (int i)
    {
        std::cout << bin_to_hex(buff1) << std::endl << bin_to_hex(buff2) << std::endl;
        std::cout << "Exception in charBufferXOR: buffers are not the same length\n";
        exit(EXIT_FAILURE);
    }
    return vec_to_string(vecXOR(vector<byte>(buff1.begin(), buff1.end()), vector<byte>(buff2.begin(), buff2.end())));
}

int _hammingCheckBits(vector<byte> bits)
{
    int count = 0;
    for (int i = 0; i < bits.size(); i++)
    {
        // cf. C Programming Language 2nd Ed. (Kernighan/Ritchie)
        int val = (int)bits[i];
        int cnt;
        for (cnt = 0; val; cnt++)
        {
            val &= val - 1; //clear least significant set bit
        }
        count += cnt;
    }
    return count;
}

int hammingDistance(secure_string str1, secure_string str2)
{
    vector<byte> checkBits = vecXOR(vector<byte>(str1.begin(), str1.end()), vector<byte>(str2.begin(), str2.end()));
    return _hammingCheckBits(checkBits);
}

int hammingDistance(string str1, string str2)
{
    vector<byte> checkBits = vecXOR(vector<byte>(str1.begin(), str1.end()), vector<byte>(str2.begin(), str2.end()));
    return _hammingCheckBits(checkBits);
}

int hammingDistance(vector<byte> str1, vector<byte> str2)
{
    vector<byte> checkBits = vecXOR(str1, str2);
    return _hammingCheckBits(checkBits);
}

template <typename T>
map<T, int> findDuplicates(vector<T> & inp)
{
    map<T, int> countMap;
    for (auto& elem : inp)
    {
        auto result = countMap.insert(std::pair<T, int>(elem, 1));
        if (!result.second)
            result.first->second++;
    }
    for (auto it = countMap.begin(); it != countMap.end();)
    {
        if (it->second == 1)
            it = countMap.erase(it);
        else
            it++;
    }
    return countMap;
}

void dumpVec(vector<byte> inp)
{
    BIO_dump_fp(stdout, (const char*)vec_to_sstring(inp).c_str(), inp.size());
}

vector<byte> _CH11_randXPend(vector<byte> inp, int premin, int premax, int postmin, int postmax)
{
    byte rByte;
    int prepend;
    int append;
    vector<byte> out;
    RAND_bytes(&rByte, 1);
    prepend = (unsigned int)rByte % (premax - premin + 1) + premin;
    RAND_bytes(&rByte, 1);
    append = (unsigned int)rByte % (postmax - postmin + 1) + premin;
    for (int i = 0; i < prepend; i++)
    {
        RAND_bytes(&rByte, 1);
        out.push_back(rByte);
    }
    out.insert(out.end(), inp.begin(), inp.end());
    for (int i = 0; i < append; i++)
    {
        RAND_bytes(&rByte, 1);
        out.push_back(rByte);
    }
    return out;
}
