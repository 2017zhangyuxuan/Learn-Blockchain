//
// Created by 张宇轩 on 2021/10/27.
//

#ifndef BLOCKCHAINWORK_ASYMMETRICKEYS_H
#define BLOCKCHAINWORK_ASYMMETRICKEYS_H
#include <string>


// 设定为一个抽象类，对外提供必要的接口
class AsymmetricKeys {
public:
    virtual std::string GetPrivateKey() = 0;                    // 获取私钥，字节形式
    virtual std::string GetPublicKey() = 0;                     // 获取公钥，字节形式
    virtual std::string Sign(const std::string& message) = 0;   // 进行签名
    virtual bool VerifyMessage(                                 // 验证签名
            const std::string& message, const std::string& signature) = 0;

};



#endif //BLOCKCHAINWORK_ASYMMETRICKEYS_H
