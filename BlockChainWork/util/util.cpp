//
// Created by 张宇轩 on 2021/10/25.
//

#include "util.h"
#include <string>
#include <algorithm>
#include<integer.h>
#include<osrng.h>
#include<eccrypto.h>
#include <oids.h>
#include <files.h>
#include <filters.h>
#include <filesystem>
#include <hex.h>
#include <ripemd.h>

using namespace CryptoPP;   // 使用密码学相关库函数

/** All alphanumeric characters except for "0", "I", "O", and "l" */
const char* Util::pszBase58 = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

bool Util::DecodeBase58(const char* psz, std::vector<unsigned char>& vch)
{
    // Skip leading spaces.
    while (*psz && isspace(*psz))
        psz++;
    // Skip and count leading '1's.
    int zeroes = 0;
    int length = 0;
    while (*psz == '1') {
        zeroes++;
        psz++;
    }
    // Allocate enough space in big-endian base256 representation.
    int size = strlen(psz) * 733 /1000 + 1; // log(58) / log(256), rounded up.
    std::vector<unsigned char> b256(size);
    // Process the characters.
    while (*psz && !isspace(*psz)) {
        // Decode base58 character
        const char* ch = strchr(pszBase58, *psz);
        if (ch == nullptr)
            return false;
        // Apply "b256 = b256 * 58 + ch".
        int carry = ch - pszBase58;
        int i = 0;
        for (std::vector<unsigned char>::reverse_iterator it = b256.rbegin(); (carry != 0 || i < length) && (it != b256.rend()); ++it, ++i) {
            carry += 58 * (*it);
            *it = carry % 256;
            carry /= 256;
        }
        assert(carry == 0);
        length = i;
        psz++;
    }
    // Skip trailing spaces.
    while (isspace(*psz))
        psz++;
    if (*psz != 0)
        return false;
    // Skip leading zeroes in b256.
    std::vector<unsigned char>::iterator it = b256.begin() + (size - length);
    while (it != b256.end() && *it == 0)
        it++;
    // Copy result into output vector.
    vch.reserve(zeroes + (b256.end() - it));
    vch.assign(zeroes, 0x00);
    while (it != b256.end())
        vch.push_back(*(it++));
    return true;
}

bool Util::DecodeBase58(const std::string& str, std::vector<unsigned char>& vchRet)
{
    return DecodeBase58(str.c_str(), vchRet);
}

std::string Util::EncodeBase58(const unsigned char* pbegin, const unsigned char* pend)
{
    // Skip & count leading zeroes.
    int zeroes = 0;
    int length = 0;
    while (pbegin != pend && *pbegin == 0) {
        pbegin++;
        zeroes++;
    }
    // Allocate enough space in big-endian base58 representation.
    int size = (pend - pbegin) * 138 / 100 + 1; // log(256) / log(58), rounded up.
    std::vector<unsigned char> b58(size);
    // Process the bytes.
    while (pbegin != pend) {
        int carry = *pbegin;
        int i = 0;
        // Apply "b58 = b58 * 256 + ch".
        for (std::vector<unsigned char>::reverse_iterator it = b58.rbegin(); (carry != 0 || i < length) && (it != b58.rend()); it++, i++) {
            carry += 256 * (*it);
            *it = carry % 58;
            carry /= 58;
        }

        assert(carry == 0);
        length = i;
        pbegin++;
    }
    // Skip leading zeroes in base58 result.
    std::vector<unsigned char>::iterator it = b58.begin() + (size - length);
    while (it != b58.end() && *it == 0)
        it++;
    // Translate the result into a string.
    std::string str;
    str.reserve(zeroes + (b58.end() - it));
    str.assign(zeroes, '1');
    while (it != b58.end())
        str += pszBase58[*(it++)];
    return str;
}

std::string Util::EncodeBase58(const std::vector<unsigned char>& vch)
{
    return EncodeBase58(vch.data(), vch.data() + vch.size());
}

// transform (InputIterator first1, InputIterator last1, OutputIterator result, UnaryOperator op)
void Util::ToLower(std::string &s) {
    transform(s.begin(),s.end(),s.begin(),::tolower); //转小写
}

void Util::ToUpper(std::string &s) {
    transform(s.begin(),s.end(),s.begin(),::toupper); //转大写
}

// 反转字节序，输入是Hex格式
std::string Util::ReverseByteOrder(const std::string& str) {
    int len = str.size();
    std::string  res = "";
    for (int i=len-2,j=0;i>=0;i-=2) {
        res.push_back(str[i]);
        res.push_back(str[i+1]);
    }
    return res;
}

std::string Util::DoubleSHA256(const std::string& hex) {
    CryptoPP::SHA256 sha1,sha2;
    std::string hash;
    StringSource ss(hex, true,
                    new HexDecoder(                // 原来是Hex编码表示，需要先解码成64字节
                    new HashFilter(sha1,
                    new HashFilter(sha2,        // 两次SHA256
                    new HexEncoder(                // Hex编码
                    new StringSink(hash))))));  // 结果输出到字符串
    return hash;
}

std::string Util::GetRandomBytes(int bytes) {
    AutoSeededRandomPool rng;       // 随机数生成器
    byte output[bytes];
    rng.GenerateBlock(output, bytes);
    return reinterpret_cast<const char *>(output);
}

// 字节形式转换为Hex编码
std::string Util::HexEncode(const std::string bytes) {
    std::string hex;
    StringSource ss(bytes, true, new HexEncoder(new StringSink(hex)));
    return hex;
}

// int 4字节 转换为Hex编码
std::string Util::HexEncode(int num, int size) {
    unsigned char* s;
    s = reinterpret_cast<unsigned char *>(&num);
    std::string hex;
    StringSource ss(s, size, true, new HexEncoder(new StringSink(hex)));
    return hex;
}

// Hex 解码成字节形式
std::string Util::HexDecode(const std::string hex) {
    std::string bytes;
    StringSource ss(hex, true, new HexDecoder(new StringSink(bytes)));
    return bytes;
}

// 输入为字节，进行SHA256哈希，返回也是字节形式
std::string Util::SHA256(const std::string bytes) {
    std::string hash;
    CryptoPP::SHA256 sha;
    StringSource ss(bytes, true,
                    new HashFilter(sha,
                    new StringSink(hash)));
    return hash;
}

// 输入为字节，进行RIPEMD160哈希，返回也是字节形式
std::string Util::RIPEMD160(const std::string bytes) {
    std::string hash;
    CryptoPP::RIPEMD160 rip160;
    StringSource ss(bytes, true,
                    new HashFilter(rip160,
                    new StringSink(hash)));
    return hash;
}


// 字节形式转换为Base58check编码
 std::string Util::EncodeBase58Check(const std::string bytes) {
    std::string hash;
    CryptoPP::SHA256 sha1,sha2;
    StringSource ss(bytes, true,                // 进行两次SHA256 哈希
                    new HashFilter(sha1,
                    new HashFilter(sha2,
                    new StringSink(hash))));
    std::string checksum = hash.substr(0, 4);   // 取前4位作为checksum
    std::string input = bytes + checksum;
    // 最后用Base58进行编码
    std::string res = EncodeBase58(reinterpret_cast<const unsigned char *>(&input[0]),
                                   reinterpret_cast<const unsigned char *>(&input[input.size()]));
    return res;
}

// Base58check解码，输出字节形式
std::string Util::DecodeBase58Check(const std::string base58) {
    std::vector<unsigned char> res;
    DecodeBase58(base58, res);
    std::string bytes(res.begin(), res.end()-4);    // 减去后面的4字节hash checksum
    return bytes;
}

