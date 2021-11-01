//
// Created by 张宇轩 on 2021/10/29.
//

#ifndef BLOCKCHAINWORK_TXIN_H
#define BLOCKCHAINWORK_TXIN_H
#include <string>
#include "json.hh"
namespace blockchain {
    class TxIn {
    public:
        unsigned int sequence;
        std::string prevOutHash;         // 对应UTXO所在交易的hash，hex格式，注意在这里还是以小端形式保存，只有输出展示时需要转换字节序
        unsigned int prevOutIndex;       // 对应UTXO所在交易 vout 的下标
        std::string scriptSig;           // 解锁脚本

        // 序列化成JSON输出
        std::string MarshalJson();

        // 序列化Hex输出
        std::string MarshalHex();

        // 内部传递json类型
        nlohmann::json Marshal();



    };
} // namespace blockchain



#endif //BLOCKCHAINWORK_TXIN_H
