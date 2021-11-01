//
// Created by 张宇轩 on 2021/10/25.
//

#ifndef BLOCKCHAINWORK_UTIL_H
#define BLOCKCHAINWORK_UTIL_H
#include <vector>

// 把加密、哈希的相关算法都放在工具类中
class Util {
public:
    // Base58编码与解码
    static bool DecodeBase58(const char* psz, std::vector<unsigned char>& vch);
    static bool DecodeBase58(const std::string& str, std::vector<unsigned char>& vchRet);
    static std::string EncodeBase58(const unsigned char* pbegin, const unsigned char* pend);
    static std::string EncodeBase58(const std::vector<unsigned char>& vch);

    // 将字符串中字符转为小写
    static void ToLower(std::string& s);
    // 将字符串中字符转为大写
    static void ToUpper(std::string& s);

    // 反转字节序，输入是Hex格式
    static std::string ReverseByteOrder(const std::string& str);

    // 输入为Hex编码，进行两次SHA256，输出为Hex编码形式 (注意：输入和输出都是小端模式)
    static std::string DoubleSHA256(const std::string& hex);

    // 输入为产生随机数的字节数
    static std::string GetRandomBytes(int bytes);

    // 字节形式转换为Hex编码
    static std::string HexEncode(const std::string bytes);

    // 对int类型进行Hex编码，还可以指定具体字节数
    static std::string HexEncode(int num, int size = 4);

    // Hex 解码成字节形式
    static std::string HexDecode(const std::string hex);

    // 输入为字节，进行SHA256哈希，返回也是字节形式
    static std::string SHA256(const std::string bytes);

    // 输入为字节，进行RIPEMD160哈希，返回也是字节形式
    static std::string RIPEMD160(const std::string bytes);

    // 字节形式转换为Base58check编码,bytes带上了前缀
    static std::string EncodeBase58Check(const std::string bytes);

    // Base58check解码，输出字节形式，输出带版本前缀，由业务方处理
    static std::string DecodeBase58Check(const std::string base58);


private:
    static const char* pszBase58;
};


#endif //BLOCKCHAINWORK_UTIL_H
