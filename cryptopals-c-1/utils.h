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
    int buffLen = buff1.length();
    string XORbuff;
    for (int i = 0; i < buffLen; i += 2)
    {
        string ap = { (char)buff1[i], (char)buff1[i + 1] };
        string bp = { (char)buff2[i], (char)buff2[i + 1] };
        char a = hex_to_bin(ap)[0];
        char b = hex_to_bin(bp)[0];
        char x = (char)((int)a ^ (int)b);
        string heXOR = bin_to_hex(string(1, x));
        XORbuff += heXOR;
    }
    return XORbuff;
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
    vector<byte> XORbuff = vecXOR(vector<byte>(buff1.begin(), buff1.end()), vector<byte>(buff2.begin(), buff2.end()));
    return string(XORbuff.begin(), XORbuff.end());
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
