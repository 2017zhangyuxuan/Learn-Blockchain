//
// Created by 张宇轩 on 2021/10/29.
//

#ifndef BLOCKCHAINWORK_MINER_H
#define BLOCKCHAINWORK_MINER_H
#include "block/Block.h"

namespace blockchain {
    // 矿工：模拟挖矿，产生区块
    class Miner {
    private:
        // 根据区块头中的 Bits 计算目标值
        std::string GetTarget(const std::string& bits);

    public:
        // 模拟挖矿，打包交易，生成区块
        Block* Mine();

        // 模拟挖矿过程，返回符合条件的nonce值
        unsigned int FindNonce(const Block& block);

        // 验证一个区块的哈希是否满足条件
        bool proveBlock(const Block& block);

    };
} // namespace blockchain



#endif //BLOCKCHAINWORK_MINER_H
