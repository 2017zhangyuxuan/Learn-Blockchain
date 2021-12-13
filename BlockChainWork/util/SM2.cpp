//
// Created by 张宇轩 on 2021/12/13.
//

#include "SM2.h"
#include <integer.h>
#include <eccrypto.h>
#include <iostream>
#include <memory>
#include <sm3.h>
#include "util.h"

using namespace std;

namespace CryptoPP {

    // 默认构造函数，使用官方参数
    SM2::SM2() {
        init();
    }

    // 初始化曲线参数，使用官方参数
    void SM2::init() {
        // 初始化曲线参数
        Integer xg, yg;
        p = std::make_shared<Integer>(
                Util::StringToInteger("FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF"));
        a = std::make_shared<Integer>(
                Util::StringToInteger("FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFC"));
        b = std::make_shared<Integer>(
                Util::StringToInteger("28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93"));
        ecp = std::make_shared<ECP>(*p, *a, *b);
        xg = Util::StringToInteger("32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7");
        yg = Util::StringToInteger("BC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0");
        n = std::make_shared<Integer>(
                Util::StringToInteger("FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123"));
        generator = std::make_shared<ECPPoint>(xg, yg);

        // 初始化公私钥
        privateKey = std::make_shared<Integer>(Util::StringToInteger("3945208F7B2144B13F36E38AC6D39F95889393692860B51A42FB81EF4DF7C5B8"));              // 私钥
        publicKey = std::make_shared<ECPPoint>(ecp->Multiply(*privateKey, *generator));    // 公钥

        // 计算当前的杂凑值
        ComputeZA();
    }

    // 定制椭圆曲线参数
    SM2::SM2(Integer pp, Integer aa, Integer bb) {
        p = std::make_shared<Integer>(pp);
        a = std::make_shared<Integer>(aa);
        b = std::make_shared<Integer>(bb);
        ecp = std::make_shared<ECP>(pp, aa, bb);
        ComputeZA();
    }


    void SM2::SetPrivateKey(Integer d) {
        privateKey = std::make_shared<Integer>(d);
        publicKey = std::make_shared<ECPPoint>(ecp->Multiply(*privateKey, *generator));
        ComputeZA();
    }

    void SM2::SetGenerator(Integer xg, Integer yg, Integer nn) {
        generator = std::make_shared<ECPPoint>(xg, yg);
        this->n = std::make_shared<Integer>(nn);
        ComputeZA();
    }

    // 计算杂凑值  ZA = H256(ENTL_A ∥ ID_A ∥ a ∥ b ∥ xG ∥ yG ∥ xA ∥ yA)
    void SM2::ComputeZA() {
        // ENTL_A ID_A 采用默认值
        std::string ENTL_A;                    //  ENTLA是IDA的比特长度转换而成的两个字节
        ENTL_A.resize(2);
        ENTL_A[0] = '\x00';
        ENTL_A[1] = '\x80';
        std::string ID_A = "1234567812345678";

        std::string aa = Util::HexDecode(Util::IntegerToString(*a));
        std::string bb = Util::HexDecode(Util::IntegerToString(*b));
        std::string xG = Util::HexDecode(Util::IntegerToString(generator->x));
        std::string yG = Util::HexDecode(Util::IntegerToString(generator->y));
        std::string xA = Util::HexDecode(Util::IntegerToString(publicKey->x));
        std::string yA = Util::HexDecode(Util::IntegerToString(publicKey->y));

        SM3 sm3;
        std::string input = ENTL_A + ID_A + aa + bb + xG + yG + xA + yA;
        StringSource ss(input, true, new HashFilter(sm3, new StringSink(ZA)));

        // 与真实数据比对
//        cout << "a :" << Util::IntegerToString(*a) << endl;
//        cout << "b :" << Util::IntegerToString(*b) << endl;
//        cout << "xG:" << Util::IntegerToString(generator->x) << endl;
//        cout << "yG:" << Util::IntegerToString(generator->y) << endl;
//        cout << "xA:" << Util::IntegerToString(publicKey->x) << endl;
//        cout << "yA:" << Util::IntegerToString(publicKey->y) << endl;
//        cout << "ZA:" << Util::HexEncode(ZA) << endl;
    }


    // 签名：输入的消息和输出的签名数据  均为字节格式，签名是由(r,s)各32字节拼接得到的
    void SM2::Sign(const std::string &message, std::string &signature) {
        Integer k = Util::GetRandomInteger(30);
        Sign(message, signature, k);
    }

    // 指定临时私钥k值， 测试用
    void SM2::Sign(const std::string &message, std::string &signature, Integer k) {
        // 先计算消息散列值
        std::string M = ZA + message;
        std::string hash;
        SM3 sm3;
        StringSource ss(M, true, new HashFilter(sm3, new StringSink(hash)));
        Integer e = Util::StringToInteger(Util::HexEncode(hash));

        // 进行签名
        ECPPoint R = ecp->Multiply(k, *generator);
        Integer r,s;
        r = (e + R.x) % (*n);
        Integer tmp = (1 + (*privateKey)).InverseMod(*n);
        s = (tmp * (k - r * (*privateKey))).Modulo(*n);
        signature = Util::HexDecode(Util::IntegerToString(r) + Util::IntegerToString(s));
    }

    // 验证：输入的消息和签名均为字节格式，输出为bool值
    bool SM2::Verify(const std::string &message, const std::string &signature) {
        // 先计算消息散列值
        std::string M = ZA + message;
        std::string hash;
        SM3 sm3;
        StringSource ss(M, true, new HashFilter(sm3, new StringSink(hash)));
        Integer e = Util::StringToInteger(Util::HexEncode(hash));

        Integer r = Util::StringToInteger(Util::HexEncode(signature.substr(0,32)));
        Integer s = Util::StringToInteger(Util::HexEncode(signature.substr(32,32)));
        Integer t = (r + s) % (*n);
        ECPPoint tmp = ecp->Add(ecp->Multiply(s,*generator), ecp->Multiply(t,*publicKey));
        Integer R = (e + tmp.x) % (*n);

        return R == r;
    }

    // 公钥恢复算法，输入消息，签名（r,s），y值是否为奇数， 输出pubKey 字节格式，由(x,y) 各32字节拼接而成
    void SM2::RecoverPublicKey(const std::string &message, bool isOdd,
                                 const Integer &r, const Integer &s, std::string &pubKey) {
        // 先计算 消息散列值 e
        std::string M = ZA + message;
        std::string hash;
        SM3 sm3;
        StringSource ss(M, true, new HashFilter(sm3, new StringSink(hash)));
        Integer e = Util::StringToInteger(Util::HexEncode(hash));

        // 恢复点 R
        Integer x = (r - e) % (*n);
        Integer y_2 = (x * x * x + (*a) * x + (*b)) ;       // 带入椭圆曲线方程

        // 模素数平方根的求解: 当  p = 4u+3, g = y^2, y= g^(u+1) % p
        Integer u = (*p-3).DividedBy(4);
        Integer y = a_exp_b_mod_c(y_2,u+1,*p);      // 返回  a^b % c
        if ((isOdd && y.IsEven()) || (!isOdd && y.IsOdd())) {       // 如果与原来的y坐标奇偶性相反
            y = *p - y;
        }
        ECP::Point R(x,y);

        Integer t = r + s;
        Integer t_1 = t.InverseMod(*n);
        ECPPoint tmp = ecp->Multiply(s, *generator);
        ECPPoint P = ecp->Multiply(t_1, ecp->Subtract(R, tmp));

        pubKey = Util::HexDecode(Util::IntegerToString(P.x) + Util::IntegerToString(P.y));
    }

}