//
// Created by 张宇轩 on 2021/10/29.
//

#ifndef BLOCKCHAINWORK_BLOCK_H
#define BLOCKCHAINWORK_BLOCK_H
#include <string>
#include <vector>
#include "transaction/Transaction.h"
#include "MerkleTree.h"
namespace blockchain {
    class Block {
    public:                             // 为了方便调试，设置为public
        // header   区块头
        unsigned int version;
        std::string hashPrevBlock;
        std::string hashMerkleRoot;
        unsigned int time;
        unsigned int bits;
        unsigned int nonce;

        // 网络传输时需要对交易进行序列化
        std::vector<Transaction*> vtx;

        // 由交易列表生成的MerkleTree
        MerkleTree* merkleRoot;

        // 区块Hash
        std::string blockHash;
    public:
        // 获取区块Hash （小端）
        std::string GetBlockHash();

        // 转换成JSON格式输出
        std::string MarshalJson();
    };
} // namespace blockchain


#endif //BLOCKCHAINWORK_BLOCK_H
