//
// Created by 张宇轩 on 2021/10/29.
//

#include "TxOut.h"
#include "json.hh"
#include "util.h"

using namespace nlohmann;
namespace blockchain {
    TxOut::TxOut() {}

    // 根据传入的value，和 公钥哈希（Hex） 来生成TxOut
    TxOut::TxOut(unsigned long long value, std::string pubKeyHash) {
        this->value = value;

        // 简单的锁定脚本格式：OP_DUP OP_HASH160 PubKeyHash OP_EQUALVERIFY OP_CHECKSIG
        scriptPubKey = std::string("76") +             // 对应OP_DUP
                                      "A9" +              // 对应OP_HASH160
                                      "14" +              // 指示公钥哈希字节长度
                                      pubKeyHash +        // 公钥哈希
                                      "88" +              // 对应OP_EQUALVERIFY
                                      "AC";               // 对应OP_CHECKSIG
    }

    // 解析锁定脚本，获取对应的比特币地址（Hex格式）
    std::string TxOut::GetPubKeyHash() {
        return scriptPubKey.substr(6, 20);
    }

    // 序列化Hex输出
    std::string TxOut::MarshalHex() {
        std::string hex;
        // value
        unsigned char* p;
        p = reinterpret_cast<unsigned char*>(&value);
        std::string input(p,p+8);
        std::string valueHex = Util::HexEncode(input);
        hex = hex + valueHex;

        // scriptPubKey length 脚本的字节数
        std::string scriptLen;
        scriptLen.resize(1);
        scriptLen[0] = scriptPubKey.size()/2;      //原本是hex编码表示，所以得到字节数需要除以2
        std::string scriptLenHex = Util::HexEncode(scriptLen);
        hex = hex + scriptLenHex;

        // 最后可以直接加上锁定脚本
        hex = hex + scriptPubKey;

        return hex;
    }

    // 序列化成JSON输出
    std::string TxOut::MarshalJson() {
        json txout;
        txout["scriptPubKey"] = scriptPubKey;
        txout["value"] = value;
        return txout.dump();
    }

    json TxOut::Marshal() {
        json txout;
        txout["scriptPubKey"] = scriptPubKey;
        txout["value"] = value;
        return txout;
    }
} // namespace blockchain