# Study-Secure-Computation Resources

## Introduction and outline
This is a beginner-friendly resource which includes various secure computation libraries. This repo aims to help you understand and implement secure computation techniques such as homomorphic encryption (HE), oblivious trasfer (OT), garble circuit (GC), arithmetic secret sharing (e.g., Beaver), boolean secret sharing (e.g.,GMW), function secret sharing (FSS), zero-knowledge proof (ZKP). Each library included in this repository can be studied independently, allowing you to learn at your own pace and focus on the techniques that interest you most. Jump into any topic, anytime!

A. Homomorphic encryption
1. [OpenFHE](https://github.com/openfheorg/openfhe-development) *comming soon!*
2. [SEAL](https://github.com/microsoft/SEAL) *comming soon!*
3. [HElib](https://github.com/homenc/HElib) *comming soon!*
4. [HEaaN](https://github.com/snucrypto/HEAAN) *comming soon!*
3. [Pyfhel](https://github.com/ibarrond/Pyfhel) *comming soon!*
4. [TFHE](https://github.com/tfhe/tfhe) *comming soon!*
5. [TFHE-rs](https://github.com/zama-ai/tfhe-rs) *comming soon!*

B. 2PC protocols
1. [emp-toolkit](https://github.com/emp-toolkit) includes OT, GC, and ZKP protocols. 
    1. [emp-ot](https://github.com/emp-toolkit/emp-ot) *In progress!*:  Oblivious Transfer, Oblivious Transfer Extension and Variations.
    2. [emp-sh2pc](https://github.com/emp-toolkit/emp-sh2pc) *In progress!*: Semi-honest 2PC based on Garble Circuit
    3. [emp-zk](https://github.com/emp-toolkit/emp-zk) *comming soon!*:  Efficient and Interactive Zero-Knowledge Proofs 
    4. [emp-ag2pc](https://github.com/emp-toolkit/emp-ag2pc) *comming soon!*: Authenticated Garbling and Efficient Maliciously Secure Two-Party Computation
2. [OTExtension](https://github.com/encryptogroup/OTExtension) *comming soon!*:  C++ OT extension implementation 
3. [ABY](https://github.com/encryptogroup/ABY) *comming soon!*: A Framework for Efficient Mixed-protocol Secure Two-party Computation, including <b><u>A</u></b>rithmetic sharing, <b><u>B</u></b>oolean sharing, and <b><u>Y</u></b>ao's garble circuit. 
4. [CrypTen](https://github.com/facebookresearch/CrypTen) includes arithmetic and boolean SS protocol.
5. [libfss](https://github.com/frankw2/libfss/tree/master) *comming soon!* : Library for performing Function Secret Sharing (FSS) 
6. [pfss](https://github.com/stealthsoftwareinc/pfss) *comming soon!*:  Function secret sharing 
7. [fss](https://github.com/myl7/fss): *comming soon!*:   Function secret sharing including distributed comparison & point functions 
8. [MP-SPDZ](https://github.com/data61/MP-SPDZ) - *comming soon!*

## Installing dependencies
Since many of the libraries listed above require common dependencies, make sure to install them successfully before proceeding.

### git
```
$ sudo apt-get update
$ sudo apt-get install -y git
```

### C/C++
The requirement includes `cmake` (for build configuration) and `build-essential` (for compiling C/C++ project).
```
$ sudo apt-get update
$ sudo apt-get install -y cmake build-essential
```