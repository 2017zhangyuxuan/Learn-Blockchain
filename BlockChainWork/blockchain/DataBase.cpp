//
// Created by 张宇轩 on 2021/10/29.
//

#include "DataBase.h"
namespace blockchain {
    // 初始化
    void DataBase::load(std::string blockPath, std::string txPath) {

    }

    // 清空所有状态
    void DataBase::clearAll() {
        // 不能简单调用clear， 还应该把申请来空间给释放了
        // 或许改用智能指针来维护更好。。。
        hashToBlock.clear();
        BlockHash.clear();
        hashToTx.clear();
        mempool.clear();
        UTXOList.clear();
        tempUTXOList.clear();
    }

    // block hash To Block   维护所有的历史区块数据
    std::unordered_map<std::string, Block*> DataBase::hashToBlock;

    std::vector<std::string> DataBase::BlockHash;

    // Tx hash To Tx  维护所有的历史交易数据
    std::unordered_map<std::string, Transaction*> DataBase::hashToTx;

    // 交易池，存放当前还没有打包的交易
    std::unordered_map<std::string, Transaction*> DataBase::mempool;

    // UTXO维护列表 ：两级map，第一层map，key：比特币地址（Hex编码），第二层key为UTXO所在交易的 交易TXID
    std::unordered_map<std::string,
            std::unordered_map<std::string, UTXO>> DataBase::UTXOList;

    // UTXO 临时列表，在处理区块时，暂时性地更新UTXO状态，当区块上链后，真正更新原来的UTXO集合
    std::unordered_map<std::string,
            std::unordered_map<std::string, UTXO>> DataBase::tempUTXOList;


} // blockchain