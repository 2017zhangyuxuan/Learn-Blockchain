//
// Created by 张宇轩 on 2021/10/30.
//

#ifndef BLOCKCHAINWORK_SIMPLEWALLET_H
#define BLOCKCHAINWORK_SIMPLEWALLET_H
#include "wallet/Wallet.h"
#include "wallet/WalletKeys.h"
#include <vector>
#include <unordered_map>
#include "transaction/UTXO.h"
namespace blockchain {
    // 简单的
    class SimpleWallet : public Wallet {
    public:                 // 为了方便调试，暂时改成public
        // 密钥管理
        std::vector<WalletKeys> keys;

        // 钱包自己维护的UTXO集合,两层map，第一层 key:对应密钥的下标， 第二层key：UTXO所在交易的TXID，value：对应UTXO
        std::unordered_map<int,
                std::unordered_map<std::string, UTXO>> UTXOList;

        // 构造待签名的 vin
        void ConstructTxIn(unsigned long long paySum,      // 支付总额
                           std::vector<TxIn>& vin,         // 待构建的vin
                           std::vector<int>& vinIndex,     // 记录下标
                           Transaction* transaction);      // 传入此次构造的交易

        // 对 vin 中的input 进行签名
        std::string SignInput(int index, Transaction* tx);

    public:
        // 默认生成一个密钥对
        SimpleWallet();
        // 生成指定数量的密钥对
        SimpleWallet(int size);

        // 获取钱包余额
        unsigned long long GetBalance();

        // 进行转账交易，输入为转账金额，和对方的比特币地址（hex编码表示），两个vector的大小应保持一致
        Transaction* SendMoney(std::vector<unsigned long long> value,
                               std::vector<std::string> address);

        // 获取一个支付地址
        std::string GetBitcoinAddr();

        // 同步更新UTXO
        void UpdateUTXO();

    };
} // namespace blockchain



#endif //BLOCKCHAINWORK_SIMPLEWALLET_H
