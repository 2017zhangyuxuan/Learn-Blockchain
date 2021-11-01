//
// Created by 张宇轩 on 2021/10/29.
//

#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
#include "Miner.h"
#include "util.h"
#include "DataBase.h"
namespace blockchain {
    /*
     * Func：根据输入的bits计算出对应的目标值
     * Input：bits 格式为8位的Hex编码，输入为小端模式
     * Ouput：输出为64位的16进制字符串 (大端模式)
     */
    std::string Miner::GetTarget(const std::string& bits) {
        std::string rervse_bits = Util::ReverseByteOrder(bits);
        std::string exp = rervse_bits.substr(0,2);
        std::string pre = rervse_bits.substr(2,6);

        char tmp = Util::HexDecode(exp)[0]; // 去一个字节表示数值
        int last_len = (tmp - 3) * 2;            // 在16进制中后面 0 的位数
        int pre_len = 64 - 6 - last_len;         // 最后需要输出64位的 16进制数，所以该值为前面补0的 位数
        std::string res = "";
        for (int i=0; i < pre_len ; i++) {
            res += "0";
        }
        res += pre;
        for (int i=0; i < last_len; i++){
            res += '0';
        }
        return res;
    }


    // 模拟挖矿函数
    unsigned int Miner::FindNonce(const Block& block) {
        std::string header = Util::HexEncode(block.version) +
                             block.hashPrevBlock +
                             block.hashMerkleRoot +
                             Util::HexEncode(block.time) +
                             Util::HexEncode(block.bits);
        std::string target = GetTarget(Util::HexEncode(block.bits));

        for (unsigned int i = 0 ; i < 0xffffffff; i++) {
            std::string header_hex = header + Util::HexEncode(i);
            std::string hash = Util::DoubleSHA256(header_hex);               // 进行两次SHA256 哈希
            std::string reverse_hash = Util::ReverseByteOrder(hash);         // 最后得到的结果再转换成大端模式，跟目标值进行比较
            if (reverse_hash < target) {
                return i;
            }
        }
        // 用MAX 值表示未找到合适的nonce
        return 0xffffffff;
    }

    // 模拟挖矿，打包交易，生成区块
    Block* Miner::Mine() {
        Block* block = new Block();

        // 从内存池中选择前100个交易，打包交易，生成默克尔树
        std::vector<std::string> vecTxHash;
        std::vector<Transaction*> vecTx;
        vecTxHash.resize(100);
        vecTx.resize(100);
        int size = 0;
        for (auto pair : DataBase::mempool) {
            // check tx ...         // 本来应该还需要对交易再进行验证，这里为了简化，略过验证步骤

            vecTxHash[size] = pair.first;
            vecTx[size++] = pair.second;
            if (size >= 100) {      // 选择100个交易
                break;
            }
        }
        block->vtx = vecTx;

        // 构造默克尔树
        MerkleTree* root = MerkleTree::MakeMerkleTree(vecTxHash);
        block->merkleRoot = root;

        // 构建区块头
        block->hashMerkleRoot = root->hash;

        // 取出当前最长链上的最后一个区块
        block->hashPrevBlock = DataBase::BlockHash[DataBase::BlockHash.size() - 1];
        block->version = 1;
        block->time = time(0);          // 获取当前时间
        block->bits = 0x1f00ffff;       // 设定难度值

        // 挖矿
        block->nonce = FindNonce(*block);

        // 生成区块，整理内存池mempool，更新UTXO
        DataBase::BlockHash.push_back(block->GetBlockHash());
        DataBase::hashToBlock[block->GetBlockHash()] = block;
        for (int i = 0; i<vecTxHash.size(); i++) {
            // 移除打包进区块的交易
            DataBase::mempool.erase(vecTxHash[i]);
            Transaction* tx = vecTx[i];
            // 移除花费掉的UTXO
            for (TxIn& txIn : tx->vin) {
                // 找到UTXO 对应的交易
                Transaction* prevTx = DataBase::hashToTx[txIn.prevOutHash];
                std::string pubKeyHash = prevTx->vout[txIn.prevOutIndex].GetPubKeyHash();
                DataBase::UTXOList[pubKeyHash].erase(prevTx->GetTxID());
            }
            // 新增 UTXO
            for (TxOut& txOut : tx->vout) {
                DataBase::UTXOList[txOut.GetPubKeyHash()][tx->GetTxID()] = UTXO(txOut);
            }
        }

        return block;
    }

    // 验证一个区块的哈希是否满足条件
    bool Miner::proveBlock(const Block& block) {
        std::string header = Util::HexEncode(block.version) +
                             block.hashPrevBlock +
                             block.hashMerkleRoot +
                             Util::HexEncode(block.time) +
                             Util::HexEncode(block.bits) +
                             Util::HexEncode(block.nonce);
        std::string target = GetTarget(Util::HexEncode(block.bits));
        Util::ToUpper(target);

        std::string hash = Util::DoubleSHA256(header);              // 内部已经会先hex解码，再进行两次SHA256 哈希，最后Hex编码
        std::string reverse_hash = Util::ReverseByteOrder(hash);    // 最后得到的结果再转换成大端模式，跟目标值进行比较
        std::cout << "Target:" << std::endl;
        std::cout << target << std::endl;
        std::cout << "Block hash:" << std::endl;
        std::cout << reverse_hash << std::endl;

        if (reverse_hash < target) {
            return true;
        } else {
            return false;
        }

    }

} // namespace blockchain