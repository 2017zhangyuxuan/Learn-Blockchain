//
// Created by 张宇轩 on 2021/10/29.
//

#ifndef BLOCKCHAINWORK_TRANSACTION_H
#define BLOCKCHAINWORK_TRANSACTION_H
#include <vector>
#include "TxIn.h"
#include "TxOut.h"
namespace blockchain {
    class Transaction {
    public:
        // 本来这些属性应该用private比较好，但为了方便获取属性，这里用了public，其他也是类似
        unsigned int version;
        std::vector<TxIn> vin;
        std::vector<TxOut> vout;
        unsigned int lockTime;

        // 将交易序列化，输出为Hex格式
        std::string Serizalize();
        // 获取交易TxID (Hex编码，小端模式）
        std::string GetTxID();

        // 将交易转化为JSON格式输出
        std::string MarshalJson();
        // 从JSON中反序列化，生成交易
        std::string UnmarshalJson();
    };

} // namespace blockchain


#endif //BLOCKCHAINWORK_TRANSACTION_H
