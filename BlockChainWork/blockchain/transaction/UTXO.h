//
// Created by 张宇轩 on 2021/10/30.
//

#ifndef BLOCKCHAINWORK_UTXO_H
#define BLOCKCHAINWORK_UTXO_H
#include "TxOut.h"

namespace blockchain {
    class UTXO : public TxOut {
    public:
        UTXO();
        UTXO(TxOut tx);
        UTXO(unsigned long long value, std::string pubKeyHash);

        std::string txId;               // 对应UTXO所在的交易
        unsigned int outputIndex;       // 对应output Index位置
    };
} // namespace blockchain



#endif //BLOCKCHAINWORK_UTXO_H
