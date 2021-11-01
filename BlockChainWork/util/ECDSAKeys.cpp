//
// Created by 张宇轩 on 2021/10/27.
//

#include "ECDSAKeys.h"
#include<integer.h>//使用Integer
#include<iostream>//使用cout、cin
#include<osrng.h>//使用AutoSeededRandomPool
#include<eccrypto.h>
#include <oids.h>
#include <files.h>
#include <filters.h>
#include <filesystem>
#include <hex.h>
#include <base32.h>
#include <ripemd.h>
#include <dsa.h>
#include "test.h"
using namespace std;

ECDSAKeys::ECDSAKeys() {
    GenerateKeys();
}
ECDSAKeys::ECDSAKeys(std::string nonce) {
    GenerateKeys(nonce);
}

void ECDSAKeys::GenerateKeys() {
    AutoSeededRandomPool prng;
    // 生成私钥
    privateKey.Initialize( prng, ASN1::secp256k1() );

    // 生成公钥
    privateKey.MakePublicKey(publicKey);

    // 生成签名器
    signer = make_shared<ECDSA<ECP, SHA256>::Signer>(privateKey);

    // 生成验证器
    verifier = make_shared<ECDSA<ECP, SHA256>::Verifier>(publicKey);

}

void ECDSAKeys::GenerateKeys(std::string nonce) {
    // 指定 private exponent  32字节随机数，生成对应的私钥
    HexDecoder decoder;             // 输入为Hex编码形式，所以先解码
    decoder.Put((byte*)&nonce[0], nonce.size());
    decoder.MessageEnd();

    Integer x;
    x.Decode(decoder, decoder.MaxRetrievable());

    // 生成对应私钥
    privateKey.Initialize(ASN1::secp256k1(), x);

    // 生成公钥
    privateKey.MakePublicKey(publicKey);

    // 生成签名器
    signer = make_shared<ECDSA<ECP, SHA256>::Signer>(privateKey);

    // 生成验证器
    verifier = make_shared<ECDSA<ECP, SHA256>::Verifier>(publicKey);
}

std::string ECDSAKeys::GetPrivateKey() {
    // 这个Exponent数值其实就是对应的32字节随机数
    Integer p = privateKey.GetPrivateExponent();
    string pK;
    StringSink ss(pK);
    p.Encode(ss, 32);
    return pK;
}

std::string ECDSAKeys::GetPublicKey() {
    const ECP::Point& q = publicKey.GetPublicElement();
    const Integer& qx = q.x;
    const Integer& qy = q.y;

    string qxBytes, qyBytes;
    StringSink sx(qxBytes),sy(qyBytes);
    qx.Encode(sx, 32);
    qy.Encode(sy, 32);

    return qxBytes + qyBytes;
}

bool ECDSAKeys::QyIsOdd() {
    const ECP::Point& q = publicKey.GetPublicElement();
    const Integer& qy = q.y;
    return qy.IsOdd();
}

bool ECDSAKeys::QyIsEven() {
    const ECP::Point& q = publicKey.GetPublicElement();
    const Integer& qy = q.y;
    return qy.IsEven();
}

std::string ECDSAKeys::Sign(const std::string &message) {
    AutoSeededRandomPool prng;
    string signature;
    StringSource s( message, true /*pump all*/,
                    new SignerFilter( prng,
                                      *signer,
                                      new StringSink( signature )
                    ) // SignerFilter
    ); // StringSource
//    cout << "signature len:" << signature.size() << " output:" << signature << endl;

    return signature;
}

std::string ECDSAKeys::SignWithDER(const std::string& message) {
    std::string signature = Sign(message);

    // 将签名（包含R，S）转换成 DER 格式
    std::string derSign;
    // Make room for the ASN.1/DER encoding
    derSign.resize(3+3+3+2+signature.size());
    size_t converted_size = DSAConvertSignatureFormat(
            (byte*) (&derSign[0]), derSign.size(), DSA_DER,
            (const byte*) (signature.data()), signature.size(), DSA_P1363);
    derSign.resize(converted_size);
//    cout << "DER len:" << derSign.size() << " DER:" << derSign << endl;
    string hexDER;
    StringSource toDER(derSign, true, new HexEncoder(new StringSink(hexDER)));
//    cout << "DER hex:" << hexDER << endl;
    return hexDER;
}

bool ECDSAKeys::VerifyMessage(const std::string &message, const std::string &signature) {
    // 进行验证
    bool result;
    ECDSA<ECP, SHA256>::Verifier verifier(publicKey);
    StringSource ss( signature+message, true /*pump all*/,
                     new SignatureVerificationFilter(
                             verifier,
                             new ArraySink( (byte*)&result, sizeof(result) )
                     ));
    return result;
}

