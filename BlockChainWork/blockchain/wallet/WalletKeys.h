//
// Created by 张宇轩 on 2021/10/27.
//

#ifndef BLOCKCHAINWORK_WALLETKEYS_H
#define BLOCKCHAINWORK_WALLETKEYS_H
#include "ECDSAKeys.h"
#include <string>

namespace blockchain {
    class WalletKeys {
    private:
//        AsymmetricKeys* keys;         // 原本想用一个抽象类的，不过比特币里密码格式跟ECDSA的密钥形式比较耦合，实现也是依赖于CryptoPP库，所以不太好弄就此作罢
        ECDSAKeys keys;                 // ECDSA对应的公私钥管理
        std::string address;            // 未压缩公钥对应的地址，字节形式存储
        std::string compressedAddress;  // 压缩公钥对应的地址，字节形式存储

        void GenAddr();                 // 生成公私钥对的时候，对应的比特币地址也已经确定下来了
    public:
        // 构造函数，随机生成密钥对
        WalletKeys();
        WalletKeys(std::string exponent);   // 指定生成的私钥

        // 获取字节形式的密钥
        std::string GetRawPrivateKey();
        // 获取Hex编码形式的密钥
        std::string GetHexPrivateKey();
        // 获取WIF格式的私钥
        std::string GetWIFPrivateKey();
        // 获取WIF-Compressed 格式的私钥
        std::string GetWIFCompressedPrivateKey();

        // 获取字节形式公钥（64字节）
        std::string GetRawPublicKey();
        // 获取Hex编码格式公钥（带上前缀04）
        std::string GetHexPublicKey();
        // 获取压缩格式公钥（前缀02或03）
        std::string GetCompressedHexPublicKey();

        // 获取使用非压缩公钥产生的比特币地址（字节形式）
        std::string GetRawAddress();
        // 获取使用非压缩公钥产生的比特币地址（Hex 编码）
        std::string GetHexRawAddress();
        // 获取使用非压缩公钥产生的比特币地址（Base58check 编码）
        std::string GetBase58CheckAddress();

        // 获取使用压缩公钥产生的比特币地址（字节形式）
        std::string GetCompressedRawAddress();
        // 获取使用压缩公钥产生的比特币地址（Hex形式）
        std::string GetHexCompressedRawAddress();
        // 获取使用压缩公钥产生的比特币地址（Base58check 编码）
        std::string GetCompressedBase58CheckAddress();
        // 获取测试网压缩公钥产生的比特币地址（Base58check 编码）
        std::string GetTestCompBase58CheckAddress();

        // 解码比特币地址，返回Hex编码；因为和钱包密钥无关，所以声明为静态
        static std::string DecodeBase58check(std::string base58Addr);

        // 签名与验证
        std::string Sign(const std::string& message);   // 进行签名，字节形式（R值和S值的拼接）
        std::string SignWithDER(const std::string& message);    // 签名结果以DER编码形式返回
        bool VerifyMessage(                             // 验证签名，签名需要是原生的拼接格式，而不是DER格式
                const std::string& message, const std::string& signature);

    };
} // namespace blockchain



#endif //BLOCKCHAINWORK_WALLETKEYS_H
