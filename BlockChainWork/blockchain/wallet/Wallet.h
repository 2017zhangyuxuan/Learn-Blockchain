//
// Created by 张宇轩 on 2021/10/29.
//

#ifndef BLOCKCHAINWORK_WALLET_H
#define BLOCKCHAINWORK_WALLET_H

#include "transaction/Transaction.h"
#include <vector>
namespace blockchain {
    class Wallet {
    public:
        // 获取钱包余额
        virtual unsigned long long GetBalance() = 0;

        // 同步更新UTXO
        virtual void UpdateUTXO() = 0;

        // 进行转账交易，输入为转账金额，和对方的比特币地址（hex编码表示）
        virtual Transaction* SendMoney(std::vector<unsigned long long> value,
                                       std::vector<std::string> address) = 0;
        // 进行转账交易，输入的地址用base58表示
        Transaction* SendMoneyWithBase58(std::vector<unsigned long long> value,
                                         std::vector<std::string> address);

    };
} // namespace blockchain



#endif //BLOCKCHAINWORK_WALLET_H
