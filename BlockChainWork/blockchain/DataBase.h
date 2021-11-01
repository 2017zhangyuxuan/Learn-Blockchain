//
// Created by 张宇轩 on 2021/10/29.
//

#ifndef BLOCKCHAINWORK_DATABASE_H
#define BLOCKCHAINWORK_DATABASE_H
#include <vector>
#include <unordered_map>
#include "transaction/Transaction.h"
#include "block/Block.h"
#include "transaction/UTXO.h"

namespace blockchain {
    // 模拟比特币节点上的数据库，用来存放内存中的交易池，当前区块数据，UTXO集合
    class DataBase {
    public:
        // 测试用加载
        static void testInit();

        // 初始化加载，输入为区块和交易的存储路径
        static void load(std::string blockPath, std::string txPath);

        // 保存区块和交易数据
        static void save(std::string blockPath, std::string txPath);

        // block hash To Block  维护所有的历史区块数据，key是区块hash，value是对应的区块数据指针
        static std::unordered_map<std::string, Block*> hashToBlock;

        // 按顺序存放的BlockHash
        static std::vector<std::string> BlockHash;

        // Tx hash To Tx  维护所有的历史交易数据，key是交易hash，value是对应的交易数据指针
        static std::unordered_map<std::string, Transaction*> hashToTx;

        // 交易池，存放当前还没有打包的交易, key是交易hash，value是对应的交易数据指针
        static std::unordered_map<std::string, Transaction*> mempool;

        // UTXO维护列表 ：两级map，第一层map，key：比特币地址（Hex编码），第二层key为UTXO所在交易的 交易TXID
        static std::unordered_map<std::string,
                        std::unordered_map<std::string, UTXO>> UTXOList;

        // UTXO 临时列表，在处理区块时，暂时性地更新UTXO状态，当区块上链后，真正更新原来的UTXO集合
        static std::unordered_map<std::string,
                        std::unordered_map<std::string, UTXO>> tempUTXOList;

        // 清空所有状态
        static void clearAll();
    };
} // namespace blockchain



#endif //BLOCKCHAINWORK_DATABASE_H
