//
// Created by 张宇轩 on 2021/10/27.
//

#ifndef BLOCKCHAINWORK_ECDSAKEYS_H
#define BLOCKCHAINWORK_ECDSAKEYS_H
#include "AsymmetricKeys.h"
#include<eccrypto.h>

using namespace CryptoPP;

// 借助CryptoPP库 来实现
class ECDSAKeys : public AsymmetricKeys {
private:
    ECDSA<ECP, SHA256>::PrivateKey privateKey;
    ECDSA<ECP, SHA256>::PublicKey publicKey;
    std::shared_ptr<ECDSA<ECP, SHA256>::Signer> signer;
    std::shared_ptr<ECDSA<ECP, SHA256>::Verifier> verifier;

public:
    ECDSAKeys();
    ECDSAKeys(std::string nonce);           // 指定随机数生成私钥，输入为Hex编码格式

    // 生成公私钥
    void GenerateKeys();
    // 指定私钥随机数，输入为Hex编码格式
    void GenerateKeys(std::string nonce);

    std::string GetPrivateKey();                    // 获取私钥，字节形式，32字节
    std::string GetPublicKey();                     // 获取公钥，字节形式，64字节，对应x和y坐标
    std::string Sign(const std::string& message);   // 进行签名，字节形式（R值和S值的拼接）
    std::string SignWithDER(const std::string& message);    // 签名结果以DER编码形式返回
    bool VerifyMessage(                             // 验证签名，签名需要是原生的拼接格式，而不是DER格式
            const std::string& message, const std::string& signature);
    // 这两个函数用来帮助生成压缩格式的公钥
    bool QyIsOdd();                                 // 公钥对应y坐标是否为奇数
    bool QyIsEven();                                // 公钥对应y坐标是否为偶数

};




#endif //BLOCKCHAINWORK_ECDSAKEYS_H
