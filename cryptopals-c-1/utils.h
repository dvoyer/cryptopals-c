#pragma once

//this is getting out of hand, now there are [three] of them!
vector<char> vecXOR(vector<char> buff1, vector<char> buff2)
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
    vector<char> XORbuff;
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
    vector<char> XORbuff = vecXOR(vector<char>(buff1.begin(), buff1.end()), vector<char>(buff2.begin(), buff2.end()));
    return string(XORbuff.begin(), XORbuff.end());
}

template <typename T>
struct zallocator
{
public:
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    pointer address(reference v) const { return &v; }
    const_pointer address(const_reference v) const { return &v; }

    pointer allocate(size_type n, const void* hint = 0) {
        if (n > std::numeric_limits<size_type>::max() / sizeof(T))
            throw std::bad_alloc();
        return static_cast<pointer> (::operator new (n * sizeof(value_type)));
    }

    void deallocate(pointer p, size_type n) {
        OPENSSL_cleanse(p, n * sizeof(T));
        ::operator delete(p);
    }

    size_type max_size() const {
        return std::numeric_limits<size_type>::max() / sizeof(T);
    }

    template<typename U>
    struct rebind
    {
        typedef zallocator<U> other;
    };

    void construct(pointer ptr, const T& val) {
        new (static_cast<T*>(ptr)) T(val);
    }

    void destroy(pointer ptr) {
        static_cast<T*>(ptr)->~T();
    }

    template <typename U>
    friend bool operator==(const zallocator<T>& a, const zallocator<U>& b)
    {
        return true;
    }
    template <typename U>
    friend bool operator!=(const zallocator<T>& a, const zallocator<U>& b)
    {
        return false;
    }
    constexpr zallocator() noexcept = default;
    template< class U>
    constexpr zallocator(const zallocator<U>& other) noexcept
    {
    }

#if __cpluplus >= 201103L
    template<typename U, typename... Args>
    void construct(U* ptr, Args&&  ... args) {
        ::new (static_cast<void*> (ptr)) U(std::forward<Args>(args)...);
    }

    template<typename U>
    void destroy(U* ptr) {
        ptr->~U();
    }
#endif
};

typedef unsigned char byte;
typedef std::basic_string<char, std::char_traits<char>, zallocator<char> > secure_string;

int _hammingCheckBits(vector<char> bits)
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
    vector<char> checkBits = vecXOR(vector<char>(str1.begin(), str1.end()), vector<char>(str2.begin(), str2.end()));
    return _hammingCheckBits(checkBits);
}

int hammingDistance(vector<char> str1, vector<char> str2)
{
    vector<char> checkBits = vecXOR(str1, str2);
    return _hammingCheckBits(checkBits);
}

secure_string s_to_secureString(string inp)
{
    secure_string out;

    for (int i = 0; i < inp.size(); i++)
        out += (inp[i]);
    return out;
}

