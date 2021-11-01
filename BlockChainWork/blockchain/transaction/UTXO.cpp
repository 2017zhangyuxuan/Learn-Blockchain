//
// Created by 张宇轩 on 2021/10/30.
//

#include "UTXO.h"
namespace blockchain {
    UTXO::UTXO() {

    }
    UTXO::UTXO(TxOut tx) : TxOut(tx) {

    }
    UTXO::UTXO(unsigned long long value, std::string pubKeyHash): TxOut(value, pubKeyHash) {

    }
} // namespace blockchain