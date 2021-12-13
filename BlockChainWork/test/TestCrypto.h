//
// Created by 张宇轩 on 2021/10/31.
//

#ifndef BLOCKCHAINWORK_TESTCRYPTO_H
#define BLOCKCHAINWORK_TESTCRYPTO_H


class TestCrypto {
public:
    // 测试交易ID hash 生成
    void Test_Hash();

    // ECDSA 生成公私钥
    void ECDSA_Generate();

    // ECDSA 公私钥加载，签名与验证
    void ECDSA_LOAD();

    // 测试大整数操作，模拟实现椭圆曲线的签名过程
    void Test_SimulateSign();

    // 测试 SM2 类
    void Test_SM2();
};


#endif //BLOCKCHAINWORK_TESTCRYPTO_H
