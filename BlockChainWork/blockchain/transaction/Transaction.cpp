//
// Created by 张宇轩 on 2021/10/29.
//

#include "Transaction.h"
#include "json.hh"
#include "util.h"
#include <iostream>

using namespace nlohmann;
namespace blockchain {

    // 将交易序列化，输出为Hex格式
    std::string Transaction::Serizalize() {
        // version
        std::string ver = Util::HexEncode(version);

        // input count
        std::string inputCount;
        inputCount.resize(1);
        inputCount[0] = char(vin.size());
        std::string inputCountHex = Util::HexEncode(inputCount);

        // 所有input的Hex编码
        std::string inputsHex;
        for(TxIn& txIn : vin) {
            inputsHex = inputsHex + txIn.MarshalHex();
        }

        // output count
        std::string outputCount;
        outputCount.resize(1);
        outputCount[0] = char(vout.size());
        std::string outputCountHex = Util::HexEncode(outputCount);

        // 所有output的Hex编码
        std::string outputsHex;
        for (TxOut& txOut : vout) {
            outputsHex = outputsHex + txOut.MarshalHex();
        }

        // lock time
        std::string locktime = Util::HexEncode(lockTime);

        // 最后把所有部分Hex编码组合起来
        std::string hex = ver + inputCountHex + inputsHex + outputCountHex + outputsHex + locktime;
        Util::ToUpper(hex);         // 跟程序适配，Hex输出大写
        return hex;
    }

    // 获取交易TxID (Hex编码，小端模式）
    std::string Transaction::GetTxID() {
        return Util::DoubleSHA256(Serizalize());
    }


    // 将交易转化为JSON格式输出
    std::string Transaction::MarshalJson() {
        json res;
        res["version"] = version;
        res["lockTime"] = lockTime;
        json vins = json::array();
        json vouts = json::array();
        // 依次把输入序列化
        for (TxIn& txIn : vin) {
            vins.push_back(txIn.Marshal());
        }
        res["inputs"] = vins;

        // 依次把输出序列化
        for (TxOut& txOut : vout) {
            vouts.push_back(txOut.Marshal());
        }
        res["outputs"] = vouts;
        // 最后序列化输出JSON
        return res.dump(4);
    }

} // namespace blockchain