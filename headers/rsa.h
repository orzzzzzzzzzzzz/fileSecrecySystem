#ifndef RSA_H
#define RSA_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
using namespace std;
typedef struct  RSA_PARAM_Tag
{
    unsigned __int64   p, q;   //两个素数，不参与加密解密运算
    unsigned __int64    f;      //f=(p-1)*(q-1)，不参与加密解密运算
    unsigned __int64    n, e;   //公匙，n=p*q，gcd(e,f)=1
    unsigned __int64    d;      //私匙，e*d=1 (mod f)，gcd(n,d)=1
    unsigned __int64    s;      //块长，满足2^s<=n的最大的s，即log2(n)
} RSA_PARAM;
//小素数表
const static long   g_PrimeTable[]=
{
    3,5,7,11,13,17,19,23,29,31,37,41,43,
    47,53,59,61,67,71,73,79,83,89,97
};
const static long   g_PrimeCount=sizeof(g_PrimeTable)/sizeof(long);
const unsigned __int64  multiplier=12747293821;
const unsigned __int64  adder=1343545677842234541;//随机数类



class RandNumber{
private:
    unsigned __int64    randSeed;
public:
    RandNumber(unsigned __int64 s=0);
    unsigned __int64    Random(unsigned __int64 n);
};


static RandNumber   g_Rnd;

//模乘运算，返回值 x=a*b mod n
inline unsigned MulMod(unsigned __int64 a, unsigned __int64 b, unsigned __int64 n)
{
    return a * b % n;
}


unsigned __int64 PowMod(unsigned __int64 &base, unsigned __int64 &pow, unsigned __int64 &n);


long RabinMillerKnl(unsigned __int64 &n);


long RabinMiller(unsigned __int64 &n, long loop);


unsigned __int64 RandomPrime(char bits);


unsigned __int64 EuclidGcd(unsigned __int64 &p, unsigned __int64 &q);


unsigned __int64 Euclid(unsigned __int64 &a, unsigned __int64 &b);


unsigned __int64 SteinGcd(unsigned __int64 &p, unsigned __int64 &q);


RSA_PARAM RsaGetParam(void);


void TestRM(void);

class RSA{
public:
    void TestRSA(char* pSrc);
    unsigned char* getdecode(){return decode;}
    unsigned long long* getencode(){return encode;}
    unsigned long getpublickey(){return x;}
    unsigned long getprivatekey(){return z;}
private:
    unsigned long long* encode;
    unsigned char* decode;
    unsigned long x,y,z;
};


#endif // RSA_H
