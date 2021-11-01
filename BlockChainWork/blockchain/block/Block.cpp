//
// Created by 张宇轩 on 2021/10/29.
//

#include "Block.h"
#include "util.h"
#include "json.hh"
using namespace nlohmann;
namespace blockchain {
    // 获取区块Hash （小端）
    std::string Block::GetBlockHash() {
        if (blockHash.empty()) {
            std::string header = Util::HexEncode(version) +
                                 hashPrevBlock +
                                 hashMerkleRoot +
                                 Util::HexEncode(time) +
                                 Util::HexEncode(bits) +
                                 Util::HexEncode(nonce);
            blockHash = Util::DoubleSHA256(header);              // 内部会先hex解码，再进行两次SHA256 哈希，最后Hex编码
        }

        return blockHash;
    }

    // 转换成JSON格式输出
    std::string Block::MarshalJson() {
        json res;
        res["version"] = version;
        res["hashPrevBlock"] = hashPrevBlock;
        res["hashMerkleRoot"] = hashMerkleRoot;
        res["time"] = time;
        res["bits"] = bits;
        res["nonce"] = nonce;

        json txId = json::array();
        // 依次把输入序列化
        for (Transaction* tx : vtx) {
            txId.push_back(tx->GetTxID());
        }
        res["txIds"] = txId;

        // 最后序列化输出JSON
        return res.dump(4);
    }
} // namespace blockchain