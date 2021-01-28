#pragma once

#include <iostream>
#include <cstdlib>
#include <cassert>
#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <tuple>

#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/ssl.h>

using std::string;
using std::map;
using std::vector;
using std::tuple;

//typedefs
#include "typedefs.h"
//representation conversions
#include "conversions.h"
//general utility functions
#include "utils.h"
//everything to break vigenere ciphers specifically
#include "vigenere.h"
//aes ecb/cbc encrypt/decrypt
#include "aes.h"
