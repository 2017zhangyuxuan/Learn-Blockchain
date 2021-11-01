//
// Created by 张宇轩 on 2021/10/29.
//

#ifndef BLOCKCHAINWORK_TXOUT_H
#define BLOCKCHAINWORK_TXOUT_H
#include <string>
#include "json.hh"

namespace blockchain {
    class TxOut {
    public:
        TxOut();
        // 根据传入的value，和 公钥哈希（Hex） 来生成TxOut
        TxOut(unsigned long long value, std::string pubKeyHash);

        // value 使用8个字节来存储
        unsigned long long value;

        // 锁定脚本
        std::string scriptPubKey;

        // 序列化Hex输出
        std::string MarshalHex();
        // 解析锁定脚本，获取对应的比特币地址（Hex格式）
        std::string GetPubKeyHash();

        // 序列化成JSON输出
        std::string MarshalJson();
        nlohmann::json Marshal();
    };
} // namespace blockchain



#endif //BLOCKCHAINWORK_TXOUT_H
