//
// Created by 张宇轩 on 2021/10/29.
//

#include "TxIn.h"
#include "json.hh"
#include "util.h"

using namespace nlohmann;
namespace blockchain {
    // 序列化成JSON输出
    std::string TxIn::MarshalJson() {
        json txin;
        txin["prevOutHash"] = Util::ReverseByteOrder(prevOutHash);
        txin["prevOutIndex"] = prevOutIndex;
        txin["scriptSig"] = scriptSig;
        txin["sequence"] = sequence;
        return txin.dump();
    }

    // 序列化Hex输出
    std::string TxIn::MarshalHex() {
        std::string hex;
        // 先前的交易可以直接加上（小端模式）
        hex = hex + prevOutHash;

        // output index
        std::string outputIndex = Util::HexEncode(prevOutIndex);
        hex = hex + outputIndex;

        // script length 脚本的字节数
        std::string scriptLen;
        scriptLen.resize(1);
        scriptLen[0] = scriptSig.size()/2;      //原本是hex编码表示，所以得到字节数需要除以2
        std::string scriptLenHex = Util::HexEncode(scriptLen);
        hex = hex + scriptLenHex;

        // signature 签名也可以直接加上
        hex = hex + scriptSig;

        // sequence
        std::string sequenceHex = Util::HexEncode(sequence);
        hex = hex + sequenceHex;

        return hex;
    }

    json TxIn::Marshal() {
        json txin;
        txin["prevOutHash"] = Util::ReverseByteOrder(prevOutHash);          // 输出使用大端
        txin["prevOutIndex"] = prevOutIndex;
        txin["scriptSig"] = scriptSig;
        txin["sequence"] = sequence;
        return txin;
    }
} // namespace blockchain